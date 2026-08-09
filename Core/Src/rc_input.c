/**
  ******************************************************************************
  * @file    rc_input.c
  * @brief   TIM3の入力キャプチャ(4ch)を使い、プロポ受信機からのPWMパルス幅を
  *          us単位で算出する。
  *
  *          方式: 各チャンネルとも最初はRISINGエッジで待ち受け、
  *          キャプチャが発生したら時刻を記録してFALLINGエッジに切り替える。
  *          次にFALLINGエッジがキャプチャされたら、その差分がパルス幅となる。
  *          (カウンタオーバーフローも考慮)
  ******************************************************************************
  */
#include "rc_input.h"
#include "tim.h"

/* 有効パルス幅の範囲[us] (これ以外の値はノイズとして無視) */
#define RC_PULSE_MIN_US 800u
#define RC_PULSE_MAX_US 2200u

typedef struct
{
  uint32_t rising_capture;   /* 直近の立ち上がりキャプチャ値 */
  uint16_t pulse_us;         /* 算出済みパルス幅[us] */
  uint8_t  waiting_rising;   /* 1: 次は立ち上がり待ち, 0: 立ち下がり待ち */
  uint32_t last_update_tick; /* 最後にパルス幅を更新したHAL_GetTick() */
} RC_ChannelState;

static RC_ChannelState rc_state[RC_NUM_CHANNELS];

static const uint32_t rc_tim_channel[RC_NUM_CHANNELS] =
{
  TIM_CHANNEL_1, /* RC_CH_AILERON  : PA6 */
  TIM_CHANNEL_2, /* RC_CH_ELEVATOR : PA7 */
  TIM_CHANNEL_3, /* RC_CH_RUDDER   : PB0 */
  TIM_CHANNEL_4  /* RC_CH_THROTTLE : PB1 */
};

void RC_Input_Init(void)
{
  for (int i = 0; i < RC_NUM_CHANNELS; i++)
  {
    rc_state[i].rising_capture   = 0;
    rc_state[i].pulse_us         = 0;
    rc_state[i].waiting_rising   = 1;
    rc_state[i].last_update_tick = 0;

    /* 念のため各チャンネルをRISINGエッジ待ちに初期化してから開始 */
    __HAL_TIM_SET_CAPTUREPOLARITY(&htim3, rc_tim_channel[i], TIM_ICPOLARITY_RISING);
    HAL_TIM_IC_Start_IT(&htim3, rc_tim_channel[i]);
  }
}

uint16_t RC_GetPulseUs(RC_ChannelIndex ch)
{
  if ((uint32_t)ch >= RC_NUM_CHANNELS)
  {
    return 0;
  }
  return rc_state[ch].pulse_us;
}

uint8_t RC_IsSignalValid(RC_ChannelIndex ch)
{
  if ((uint32_t)ch >= RC_NUM_CHANNELS)
  {
    return 0;
  }
  return ((HAL_GetTick() - rc_state[ch].last_update_tick) < RC_SIGNAL_TIMEOUT_MS) ? 1 : 0;
}

/* TIM3の入力キャプチャ割込みが発生するたびにHALから呼ばれるコールバック */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  int idx;
  uint32_t active_channel;

  if (htim->Instance != TIM3)
  {
    return;
  }

  switch (htim->Channel)
  {
    case HAL_TIM_ACTIVE_CHANNEL_1: idx = 0; active_channel = TIM_CHANNEL_1; break;
    case HAL_TIM_ACTIVE_CHANNEL_2: idx = 1; active_channel = TIM_CHANNEL_2; break;
    case HAL_TIM_ACTIVE_CHANNEL_3: idx = 2; active_channel = TIM_CHANNEL_3; break;
    case HAL_TIM_ACTIVE_CHANNEL_4: idx = 3; active_channel = TIM_CHANNEL_4; break;
    default: return;
  }

  uint32_t captured = HAL_TIM_ReadCapturedValue(htim, active_channel);
  RC_ChannelState *st = &rc_state[idx];

  if (st->waiting_rising)
  {
    /* 立ち上がりエッジ: 開始時刻を記録し、次は立ち下がりを待つ */
    st->rising_capture = captured;
    st->waiting_rising = 0;
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, active_channel, TIM_ICPOLARITY_FALLING);
  }
  else
  {
    /* 立ち下がりエッジ: パルス幅を計算 (ARRオーバーフローも考慮) */
    uint32_t pulse;
    if (captured >= st->rising_capture)
    {
      pulse = captured - st->rising_capture;
    }
    else
    {
      pulse = (htim->Instance->ARR + 1u) - st->rising_capture + captured;
    }

    if (pulse >= RC_PULSE_MIN_US && pulse <= RC_PULSE_MAX_US)
    {
      st->pulse_us = (uint16_t)pulse;
      st->last_update_tick = HAL_GetTick();
    }
    /* 範囲外の値(ノイズ等)は破棄し、直前の値を保持する */

    st->waiting_rising = 1;
    __HAL_TIM_SET_CAPTUREPOLARITY(htim, active_channel, TIM_ICPOLARITY_RISING);
  }
}

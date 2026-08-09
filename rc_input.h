/**
  ******************************************************************************
  * @file    rc_input.h
  * @brief   プロポ受信機(Futaba R3206SBM等)からのPWM入力(1ch=1本の信号線)を
  *          TIM3の入力キャプチャで読み取るためのモジュール
  ******************************************************************************
  */
#ifndef __RC_INPUT_H
#define __RC_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* チャンネル数と割り当て (TIM3 CH1-4 / PA6, PA7, PB0, PB1) */
#define RC_NUM_CHANNELS 4

typedef enum
{
  RC_CH_AILERON  = 0, /* PA6 (TIM3_CH1) */
  RC_CH_ELEVATOR = 1, /* PA7 (TIM3_CH2) */
  RC_CH_RUDDER   = 2, /* PB0 (TIM3_CH3) */
  RC_CH_THROTTLE = 3  /* PB1 (TIM3_CH4) */
} RC_ChannelIndex;

/* 受信信号ロス判定のタイムアウト[ms] */
#define RC_SIGNAL_TIMEOUT_MS 100

/* 入力キャプチャを開始する (MX_TIM3_Init() の後に呼ぶこと) */
void RC_Input_Init(void);

/* 指定チャンネルの最新パルス幅[us]を取得 (未受信時は0) */
uint16_t RC_GetPulseUs(RC_ChannelIndex ch);

/* 指定チャンネルの信号が有効(タイムアウトしていない)かどうか */
uint8_t RC_IsSignalValid(RC_ChannelIndex ch);

#ifdef __cplusplus
}
#endif

#endif /* __RC_INPUT_H */

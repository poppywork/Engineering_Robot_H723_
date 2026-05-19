#include "store.h"
#include <string.h>

static const uint16_t store_pwm_values[3] = {1500, 612, 2388};

StoreUnit store_unit1 = {
    .current_pos = Store_NO1,
    .slot_status = {0, 0, 0},
    .moving = 0,
    .move_start_tick = 0
};
StoreUnit store_unit2 = {
    .current_pos = Store_NO1,
    .slot_status = {0, 0, 0},
    .moving = 0,
    .move_start_tick = 0
};

void store_init(void)
{
    store_unit1.current_pos = Store_NO1;
    memset(store_unit1.slot_status, 0, sizeof(store_unit1.slot_status));
    store_unit1.moving = 0;
    store_unit2.current_pos = Store_NO1;
    memset(store_unit2.slot_status, 0, sizeof(store_unit2.slot_status));
    store_unit2.moving = 0;
}

static void store_unit_ctrl(StoreUnit *unit, TIM_HandleTypeDef *htim, uint32_t channel)
{
    __HAL_TIM_SET_COMPARE(htim, channel, store_pwm_values[unit->current_pos]);

    if (unit->moving) {
        if ((HAL_GetTick() - unit->move_start_tick) >= STORE_MOVE_DELAY_MS) {
            unit->moving = 0;
        }
    }
}

/**
 * @brief 控制舵机 PWM
 */
void store_ctrl(void)
{
    store_unit_ctrl(&store_unit1, &htim1, TIM_CHANNEL_1);
    store_unit_ctrl(&store_unit2, &htim1, TIM_CHANNEL_3);
}

/**
 * @brief 查找空/占位槽
 */
Store_mode_e store_find_empty_slot(StoreUnit *unit)
{
    for (uint8_t i = 0; i < 3; i++) {
        if (unit->slot_status[i] == 0) {
            return (Store_mode_e)i;
        }
    }
    return Store_NO1;
}

/**
 * @brief 判断是否有空/占位槽
 */
uint8_t store_has_empty_slot(StoreUnit *unit)
{
    for (uint8_t i = 0; i < 3; i++) {
        if (unit->slot_status[i] == 0) return 1;
    }
    return 0;
}

Store_mode_e store_find_occupied_slot(StoreUnit *unit)
{
    for (uint8_t i = 0; i < 3; i++) {
        if (unit->slot_status[i] == 1) {
            return (Store_mode_e)i;
        }
    }
    return Store_NO1;
}

uint8_t store_has_occupied_slot(StoreUnit *unit)
{
    for (uint8_t i = 0; i < 3; i++) {
        if (unit->slot_status[i] == 1) return 1;
    }
    return 0;
}

/**
 * @brief 旋转到指定位置
 */
void store_rotate_to(StoreUnit *unit, Store_mode_e target)
{
    if (unit->current_pos != target) {
        unit->current_pos = target;
        unit->moving = 1;
        unit->move_start_tick = HAL_GetTick();
    }
}

/**
 * @brief 判断舵机是否到位
 */
uint8_t store_ready(StoreUnit *unit)
{
    return !unit->moving;
}

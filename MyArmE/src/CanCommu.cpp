#include "CanCommu.h"

can_message_t message_TX;
can_message_t message_RX;
bool debug::debug_state = 0;
/**
 * @brief construction
 * initparam
 */
CanCommu::CanCommu(/* args */)
{
    InitParam(); 
}

CanCommu::~CanCommu()
{
    
}

/**
 * @brief initcan,set filter rx len
 * per init,firstly initparam
 * @param can_rx 
 * @param can_tx 
 * @param baud 1M
 * @return true start can ok
 * @return false can fail
 */
bool CanCommu::InitCan(uint8_t can_rx, uint8_t can_tx, int baud)
{
    InitParam(); 
    can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT((gpio_num_t)CAN_TX, (gpio_num_t)CAN_RX, CAN_MODE_NORMAL);
    can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();
    can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
    // 配置 CAN 过滤器
    /*can_filter_config_t f_config = {
        .acceptance_code = 0x181 << 21,  // 过滤器的匹配码
        .acceptance_mask = 0xFFE00000,    // 过滤器的屏蔽码，设置过滤的位为1
        .single_filter = true,             // 设置为单个过滤器
    };*/
    g_config.rx_queue_len = CAN_RECV_RX_LEN;
    //Install CAN driver
    int err = can_driver_install(&g_config, &t_config, &f_config);
    if (err == ESP_OK) {
        can_driver_installed = true;
        if (debug::debug_state)
            printf("Driver installed\n");
    } else {
        if (debug::debug_state)
            printf("Failed to install driver %d\n", err);
        return false;
    }
    //Start CAN driver
    if (can_start() == ESP_OK) {
        if (debug::debug_state)
            printf("Driver started\n");
    } else {
    if (debug::debug_state)
            printf("Failed to start driver\n");
        return false;
    }
    return true;
}

/**
 * @brief stop can
 * 
 * @return true stop ok
 * @return false stop fail
 */
bool CanCommu::StopCan()
{
    //防止反复卸载，安装状态才可以卸载
    if (can_stop() == ESP_OK || can_driver_installed) {
        if (debug::debug_state)
            Serial.printf("into stopcan err success\n");
        int state = can_driver_uninstall();
        if (state != ESP_OK) {
            if (debug::debug_state)
                Serial.printf("stopcan err %d\n", state);
            return false;
        }
        can_driver_installed = false;
    }
    if (debug::debug_state)
        Serial.printf("stopcan err success\n");
    return true;
}

/**
 * @brief send can msg
 * 
 * @param data list
 * @param len data len
 * @param ID 0-255
 * @return true send success
 * @return false send fail
 */
bool CanCommu::SendMsg(uint8_t *data, uint8_t len, int ID)
{
    if (debug::debug_state == 1)
    {
        Serial.printf("SendMsg %d\n", ID);
    }
    bool is_send_ok = true;

    message_TX.identifier = ID;
    message_TX.extd = 0;
    message_TX.data_length_code = len;

    for (int i = 0; i < len; i++) 
    {
        message_TX.data[i] = data[i];
    }

    //Queue message for transmission overtime 5ms,can let right arm quickly check scream 1s is too long
    if ((can_send_state = can_transmit(&message_TX, pdMS_TO_TICKS(CAN_SEND_MSG_TIMEOUT))) == ESP_OK) {
        if (debug::debug_state == 1)
        {
            printf("Message queued for transmission\n");
        }
        return true;
    } else {
        printf("ERROR\n");
#if (defined MAIN_DEBUG) && (!defined ROBOT_B450)
        printf("can send err %d\n", can_send_state);
#endif
        //communilate send err
        is_send_ok = false;
        if (ID > 0 && ID <= MOTOR_NUMS &&
            send_err_count[ID-1] < MAX_LOSS_COUNT) {
                ++send_err_count[ID-1];
                // Serial.printf("send err %d -- %d\n", id, motor_state.send_err_count[id-1]);
        } 
    }
    return is_send_ok;
}

/**
 * @brief get msg
 * 
 * @param data list
 * @param len data len
 * @param ID 0-255
 * @param read_buf addr 
 * @param only_read 0/1 0-not send data 1-need send data,then motor return
 */
void CanCommu::ReadMsg(uint8_t *data, uint8_t len, int ID, std::pair<int, std::vector<uint8_t>> &read_buf, bool only_read)
{
    if (debug::debug_state == 1)
        printf("read msg %x\n\r", ID);

    if (!only_read)
        SendMsg(data, len, ID);

    message_RX.extd = 0;
 
    can_receive(&message_RX, pdMS_TO_TICKS(CAN_RECEIVE_TIMEOUT));

    read_buf.first = message_RX.identifier;
    printf("%x ",read_buf.first);

    if (debug::debug_state == 1)
    {
        Serial.print(message_RX.identifier, HEX);
        Serial.print(" ");
    }

    if (!(message_RX.flags && TWAI_MSG_FLAG_NONE)) {
        for (int i = 0; i < message_RX.data_length_code; i++) {
            read_buf.second[i] = message_RX.data[i];
            //if (debug::debug_state == 1)
            //{
                printf("%02x ", read_buf.second[i]);
            //}
        }
        if (debug::debug_state == 1)
        {
            Serial.println();
        }
    }
    return ;
}

/**
 * @brief init member variable
 * 
 */
void CanCommu::InitParam()
{
    debug::debug_state = 0;
    can_driver_installed = false;
    can_send_state = false;
    for (int i = 0; i < MOTOR_NUMS; ++i)
        send_err_count[i] = 0;
    return;
}
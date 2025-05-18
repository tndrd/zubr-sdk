#pragma once

//Версия сообщения
#define CS_MESSAGE_VERSION     1

//Команды
#define CS_CMD_MSG_CONTROL     0    //!< Управление 16бит, возвращает состояние 2*16бит
#define CS_CMD_MSG_INFO        1    //!< Получить информацию, возвращает набор параметров 3*16бит
#define CS_CMD_MSG_CONTROL_EX  2    //!< Управление 16бит, резерв 16бит, возвращает 3*16бит, 8бит, 7бит crc
#define CS_CMD_MSG_WRITE       5    //!< Запись параметра (индекс 16бит, значение 32бит, возвращает записанное значение 32бит)
#define CS_CMD_MSG_READ        6    //!< Чтение параметра (индекс 16бит, возвращает значение 32бит)
#define CS_CMD_MSG_FLASH       7    //!< Прошивка (адрес 32бит, значение 32бит)

//Длина сообщения прошивки
#define CS_CMD_FLASH_LENGTH   12

//                             CTRL INFO RSV        WR RD FLASH
//                              0    1    2  3  4    5  6  7
#define CS_CMD_LENGHTS        { 5,   2,   7, 0, 0,   9, 5, CS_CMD_FLASH_LENGTH } //!< Длины команд


//Сигнатуры устройств
#define CS_SIGNATURE_CONFIG  1939 //!< Сигнатура конфигурации
#define CS_SIGNATURE_MFLASH  1940 //!< Загрузчик для серводвигателей в металлическом и пластиковом корпусах
#define CS_SIGNATURE_MOTOR   1946 //!< Стандартный двигатель в металлическом корпусе
#define CS_SIGNATURE_LMOTOR  1948 //!< Двигатель в пластиковом корпусе
#define CS_SIGNATURE_MOTOR21 1949 //!< Серводвигатель в металлическом корпусе с 21-битным энкодером
#define CS_SIGNATURE_BLDC    1950 //!< Серводвигатель BLDC
#define CS_SIGNATURE_TENSO   1812
#define CS_SIGNATURE_FORCE   1905 //!< Датчик усилия
#define CS_SIGNATURE_HOLL    1904 //!< Измеритель на основе датчиков Холла

//Коды ошибок прошивки
#define CS_UE_NONE             0 //!< Нету ошибок
#define CS_UE_SWITCH           1 //!< Ошибка переключения в режим прошивки
#define CS_UE_ERASE            2 //!< Ошибка стирания памяти
#define CS_UE_FLASH          100 //!< Адрес ошибки прошивки


//Кодовая книга
//Для всех устройств
#define CS_CB_SIGNATURE        0 //!< Сигнатура устройства
#define CS_CB_VERSION          1 //!< Версия программы
#define CS_CB_DEVICE_ID        2 //!< Идентификатор устройства
#define CS_CB_PROTOCOL_ID      3 //!< Идентификатор протокола обмена с устройством
#define CS_CB_DEVICE_MODE      4 //!< Режим работы устройства

#define CS_CB_UART_ZUBR_BASE   5
#define CS_CB_BAUDRATE         5 //!< Скорость обмена


//Загрузчик Flash серводвигателей в металлическом и пластиковом корпусах
#define CS_CB_START_PROG      10 //!< Запустить рабочую программу
#define CS_CB_INIT_CONFIG     11 //!< Стереть записанную конфигурацию, она будет по умолчанию
#define CS_CB_RESET_MODE      12 //!< Установить режим по умолчанию
#define CS_CB_SET_PROTOCOL    13 //!< Установить заданный протокол обмена
#define CS_CB_ERASE_PROG      14 //!< Стереть записанную программу


//Наборы для различных устройств
//Специфические параметры модуля IMU
#define CS_CB_TIME_SEC_LOW    10 //!< Секунды штампа времени
#define CS_CB_X               11
#define CS_CB_Y               12
#define CS_CB_Z               13
#define CS_CB_W               14
#define CS_CB_ACC             15


//Специфические параметры измерителя на датчиках холла
#define CS_CB_HOLL_CH0            10
#define CS_CB_HOLL_CH1            11
#define CS_CB_HOLL_CH2            12


//Специфические параметры двигателя
#define CS_CB_ANGLE_BASE          10

#define CS_CB_ADC_BASE           200

#define CS_CB_TARGET_PWM         100 //Значение ШИМ
#define CS_CB_CONTROL_VALUE      500 //Значение целевого значения регулятора

#define CS_CB_RANGLE_PID_BASE     60 //Регулятор позиции
#define CS_CB_RANGLE_VELO_BASE   110 //Регулятор скорости

#define CS_CB_RLIGHT_VELO_BASE   220
#define CS_CB_RLIGHT_SPRING_BASE 240

#define CS_CB_RMOMENT_VELO_BASE  300
#define CS_CB_RMOMENT_FRIC_BASE  310

#define CS_CB_RCALIBR_VELO_BASE  350
#define CS_CB_RCALIBR_PID_BASE   370

#define CS_CB_RANGLE_T_BASE      600

#define CS_CB_RPID_PID_BASE      700
#define CS_CB_RPID_VELO_BASE     740

//Специфические параметры двигателя с измерителем тока и 21-битным энкодером
#define CS_CB_CURRENT_BASE       250 //Регулятор тока

//Специфические параметры измерителя усилия
#define CS_CB_FORCE_TOP           10
#define CS_CB_FORCE_BOT           11
#define CS_CB_FORCE_TOP_MIN       12
#define CS_CB_FORCE_TOP_MAX       13


//Диапазон воздействий не покрывает весь диапазон значений, отправляемый в команде
// "воздействие". Поэтому мы можем специальными числами передавать в данной команде
// особые режимы работы устройств
#define CS_UM_FREE                 32767 //!< Шим от двигателя отключен
#define CS_UM_HOLD                 32766 //!< Удерживаем текущую позицию
#define CS_UM_SOFT                 32765 //!< Удерживаем текущую позицию давая возможность его изменить (пластилин)
#define CS_UM_NONE                 32764 //!< Значение не задано
#define CS_UM_SET_ZERO             32763 //!< Зафиксировать нулевую позицию
#define CS_UM_ZERO_UNLOCK          32762 //!< Разблокировать фиксацию нулевой позиции
#define CS_UM_ZP_UNLOCK            32761 //!< Разблокировать функцию настройки нулевой позиции
#define CS_UM_ZP_LOCK              32760 //!< Заблокировать функцию настройки нулевой позиции
#define CS_UM_ZP_SET_FACTORY       32759 //!< Установить заводской нулевой угол и диапазон +-150градусов
#define CS_UM_ZP_SET_NULL          32758 //!< Установка нулевого угла относительно нулевого заводского
#define CS_UM_ZP_SET_BEGIN         32757 //!< Установить значение граничного начального угла относительно нулевого
#define CS_UM_ZP_SET_END           32756 //!< Установить значение граничного конечного угла относительно нулевого



//Смещение угла для возможности регулировки в диапазоне
#define CS_ANGLE_OFFSET             1000

//Диапазон регулировки углов
#define CS_ANGLE_MIN                   0 //!< Минимальный угол регулирования 0 градусов
#define CS_ANGLE_MAX               14000 //!< Максимальный угол регулирования 307.6 градуса

//При подаче чисел в диапазоне 20000+-4000 система отправляет их прямиком на PWM
#define CS_PWM_CENTRAL             20000 //!< Нулевое значение ШИМ
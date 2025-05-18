#include "CsMessage.h"

namespace Zubr {

//!
//! \brief addIntN Добавить N-битное значение
//! \param val     Значение
//! \param bits    Количество бит значения
//!
void CsMessageOut::addIntN(int val, int bits) {
  //Начальная часть
  val &= 0xffffffffu >> (32 - bits);
  //Добавляем биты в текущий байт
  mBuffer[mPtr] |= (val << mUsedBits) | 0x80;

  //Количество битов добавленных выше
  int appendedBits = (bits < 7 ? bits : 7) - mUsedBits;
  val >>= appendedBits;
  bits -= appendedBits;

  mUsedBits += appendedBits;
  if (mUsedBits >= 7) {
    mUsedBits = 0;
    mPtr++;
  }

  //Целая часть
  while (bits > 7) {
    mBuffer[mPtr++] = val | 0x80;
    val >>= 7;
    bits -= 7;
  }

  //Заключительная часть
  if (bits) {
    mUsedBits = bits;
    mBuffer[mPtr] = (val & (0xff >> (8 - bits))) | 0x80;
  }
}

//!
//! \brief addInt8 Добавить 8-битное значение
//! \param val 8-битное значение
//!
void CsMessageOut::addInt8(int val) {
  //Добавляем биты в текущий байт
  mBuffer[mPtr++] |= (val << mUsedBits) | 0x80;

  //Количество битов добавленных выше
  int bits = 7 - mUsedBits;

  //Количество использованных битов в новом байте
  mUsedBits = 8 - bits;
  mBuffer[mPtr] = ((val & 0xff) >> bits) | 0x80;
  if (mUsedBits == 7) {
    mPtr++;
    mBuffer[mPtr] = 0;
    mUsedBits = 0;
  }
}

//!
//! \brief addInt7 Добавить 7-битное значение
//! \param val      7-битное значение
//!
void CsMessageOut::addInt7(int val) {
  //Добавляем биты в текущий байт
  mBuffer[mPtr++] |= (val << mUsedBits) | 0x80;

  //Количество битов добавленных выше
  int bits = 7 - mUsedBits;

  //Количество использованных битов в новом байте
  if (bits != 7)
    mBuffer[mPtr] = ((val & 0x7f) >> bits) | 0x80;
}

//!
//! \brief addInt16 Добавить 16-битное значение
//! \param val 16-битное значение
//!
void CsMessageOut::addInt16(int val) {
  addInt8(val);
  addInt8(val >> 8);
}

//!
//! \brief addInt32 Добавить 32-битное значение
//! \param val 32-битное значение
//!
void CsMessageOut::addInt32(int val) {
  addInt8(val);
  addInt8(val >> 8);
  addInt8(val >> 16);
  addInt8(val >> 24);
}

//!
//! \brief addFloat Добавить 32-битное с плавающей точкой
//! \param val      32-битное с плавающей точкой
//!
void CsMessageOut::addFloat(float val) { addInt32(floatToUInt(val)); }

//!
//! \brief addBlock Добавить блок байтов
//! \param block    Блок байтов
//! \param size     Размер блока
//!
void CsMessageOut::addBlock(const char *block, int size) {
  while (size--)
    addInt8(*block++);
}

//!
//! \brief beginQuery Инициализация буфера командой cmd. После инициализации
//! можно добавлять данные \param cmd        Формируемая команда \param id
//! Идентификатор устройства, которому адресована данная команда
//!
void CsMessageOut::beginQuery(char cmd, int id) {
  mUsedBits = 0;
  mPtr = 1;
  mBuffer[0] = ((cmd << 4) & 0x70) | (id & 0xf);
  mBuffer[1] = 0;
}

//!
//! \brief beginAnswer Инициализация буфера для ответа. После инициализации
//! можно добавлять данные
//!
void CsMessageOut::beginAnswer() {
  mUsedBits = 0;
  mPtr = 0;
  mBuffer[0] = 0;
}

//!
//! \brief hostBeginQuery Инициализация буфера командой cmd. После инициализации
//! можно добавлять данные \param cmd            Формируемая команда
//!
void CsMessageOut::hostBeginQuery(char cmd) {
  mUsedBits = 0;
  mPtr = 1;
  mBuffer[0] = ((cmd)&0x7f);
  mBuffer[1] = 0;
}

//!
//! \brief hostEnd Завершение формирования команды, дописывание контрольной
//! суммы и символа \n
//!
void CsMessageOut::hostEnd() {
  //Завершаем и КС
  end();
  mBuffer[mPtr++] = '\n';
  mBuffer[mPtr] = 0;
}

//!
//! \brief end Завершение формирования команды и дописывание контрольной суммы
//!
void CsMessageOut::end() {
  if (mUsedBits)
    mPtr++;
  mBuffer[mPtr] = crc(mBuffer, mPtr) | 0x80;
  mBuffer[++mPtr] = 0;
}

//!
//! \brief makeAnswerControl Сформировать ответ на команду "Управление"
//! \param angle             Текущий угол сервы
//! \param moment            Текущий момент
//!
void CsMessageOut::makeAnswerControl(int angle, int moment) {
  beginAnswer();
  addInt16(angle);
  addInt16(moment);
  end();
}

void CsMessageOut::makeAnswerControlEx(int angle, int pwm, int velo,
                                       int reserv8, int flags) {
  beginAnswer();
  addInt16(angle);
  addInt16(pwm);
  addInt16(velo);
  addInt8(reserv8);
  addInt7(flags);
  end();
}

//!
//! \brief makeAnswerInfo  Сформировать ответ на команду "Получить информацию"
//! \param val0            Значение 0
//! \param val1            Значение 1
//! \param val2            Значение 2
//!
void CsMessageOut::makeAnswerInfo(int val0, int val1, int val2) {
  beginAnswer();
  addInt16(val0);
  addInt16(val1);
  addInt16(val2);
  end();
}

//!
//! \brief makeAnswerWrite Сформировать ответ на команду "Запись параметра"
//! \param value           Значение, записанное в параметр
//!
void CsMessageOut::makeAnswerWrite(int value) {
  beginAnswer();
  addInt32(value);
  end();
}

//!
//! \brief makeAnswerRead Сформировать ответ на команду "Чтение параметра"
//! \param index          Индекс параметра
//! \param value          Значение параметра
//!
void CsMessageOut::makeAnswerRead(int value) {
  beginAnswer();
  // addInt16( index );
  addInt32(value);
  end();
}

//!
//! \brief crc  Вычисление контрольной суммы для блока данных
//! \param buf  Буфер с данными, на которых вычисляется контрольная сумма
//! \param size Размер данных в байтах
//! \return     Контрольная сумма
//!
int CsMessageOut::crc(const char *buf0, int size0, const char *buf1,
                      int size1) {
  /*
    Name  : CRC-8
    Poly  : 0x31	x^8 + x^5 + x^4 + 1
    Init  : 0xFF
    Revert: false
    XorOut: 0x00
    Check : 0xF7 ("123456789")
    MaxLen: 15 байт (127 бит) - обнаружение
      одинарных, двойных, тройных и всех нечетных ошибок
  */
  static const unsigned char Crc8Table[256] = {
      0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97, 0xB9, 0x88, 0xDB, 0xEA,
      0x7D, 0x4C, 0x1F, 0x2E, 0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
      0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D, 0x86, 0xB7, 0xE4, 0xD5,
      0x42, 0x73, 0x20, 0x11, 0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
      0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52, 0x7C, 0x4D, 0x1E, 0x2F,
      0xB8, 0x89, 0xDA, 0xEB, 0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
      0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13, 0x7E, 0x4F, 0x1C, 0x2D,
      0xBA, 0x8B, 0xD8, 0xE9, 0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
      0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C, 0x02, 0x33, 0x60, 0x51,
      0xC6, 0xF7, 0xA4, 0x95, 0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
      0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6, 0x7A, 0x4B, 0x18, 0x29,
      0xBE, 0x8F, 0xDC, 0xED, 0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
      0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE, 0x80, 0xB1, 0xE2, 0xD3,
      0x44, 0x75, 0x26, 0x17, 0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
      0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2, 0xBF, 0x8E, 0xDD, 0xEC,
      0x7B, 0x4A, 0x19, 0x28, 0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
      0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0, 0xFE, 0xCF, 0x9C, 0xAD,
      0x3A, 0x0B, 0x58, 0x69, 0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
      0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A, 0xC1, 0xF0, 0xA3, 0x92,
      0x05, 0x34, 0x67, 0x56, 0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
      0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15, 0x3B, 0x0A, 0x59, 0x68,
      0xFF, 0xCE, 0x9D, 0xAC};

  unsigned char crc = 0xFF;

  //Вычисляем КС для первой части буфера
  while (size0--)
    crc = Crc8Table[crc ^ ((*buf0++) & 0xff)];

  //Вычисляем КС для второй части буфера
  while (size1--)
    crc = Crc8Table[crc ^ ((*buf1++) & 0xff)];

  return crc;
}

CsMessageIn::CsMessageIn(const char *buf, short start, short size, int ptr)
    : mBuffer(buf), mStart(start), mBufSize(size), mPtr(ptr), mUsedBits(0) {}

//!
//! \brief reset Установить новое начало данных и сбросить номер текущего байта
//! и счетчик битов \param start Индекс начала новых данных \param ptr   Индекс,
//! с которого начинаем декодирование
//!
void CsMessageIn::reset(int start, int ptr) {
  mStart = start;
  mPtr = ptr;
  mUsedBits = 0;
}

//!
//! \brief getUInt8 Извлекает 8-битное число предполагая, что оно беззнаковое
//! \return         8-битное число
//!
//!Базовая функция извлечения
//!Сначала получаем младшую половину из одного байта
//!затем старшую половину - из следующего байта
int CsMessageIn::getUInt8() {
  //Получаем первую порцию данных
  int val = (at(mPtr++) & 0x7f) >> mUsedBits;
  //Количество считанных битов
  int bits = 7 - mUsedBits;
  //Оставшиеся биты
  val |= ((at(mPtr) & 0x7f) << bits) & 0xff;
  //Сколько было оставшихся битов (забрали из текущего байта)
  mUsedBits = 8 - bits;
  if (mUsedBits == 7) {
    //Из текущего байта забрали все
    //переходим к следующему байту
    mPtr++;
    mUsedBits = 0;
  }
  return val;
}

//!
//! \brief getInt8 Извлекает 8-битное число
//! \return        8-битное число со знаком
//!
int CsMessageIn::getInt8() {
  //Преобразуем беззнаковое 8-битное число к 32-разрядному числу со знаком
  int8_t val = getUInt8();
  // if( val & 0x80 ) val -= 256;
  return val;
}

//!
//! \brief getUInt7 Извлекает 7-битное число
//! \return         7-битное число без знака
//!
int CsMessageIn::getUInt7() {
  //Получаем первую порцию данных
  int val = (at(mPtr++) & 0x7f) >> mUsedBits;
  //Количество считанных битов
  int bits = 7 - mUsedBits;
  if (bits != 7) {
    //Оставшиеся биты
    val |= ((at(mPtr) & 0x7f) << bits) & 0x7f;
  }
  return val;
}

//!
//! \brief getUInt16 Извлекает 16-битное число предполагая, что оно беззнаковое
//! \return          16-битное число
//!
int CsMessageIn::getUInt16() {
  int val = getUInt8();
  val |= getUInt8() << 8;
  return val;
}

//!
//! \brief getInt16 Извлекает 16-битное число со знаком
//! \return         16-битное число со знаком
//!
int CsMessageIn::getInt16() {
  //Преобразуем беззнаковое 8-битное число к 32-разрядному числу со знаком
  int16_t val = getUInt16();
  // if( val & 0x8000 ) val -= 65536;
  return val;
}

//!
//! \brief getInt32 Извлекает 32-битное число
//! \return 32-битное число
//!
int CsMessageIn::getInt32() {
  int32_t val = getUInt8();
  val |= getUInt8() << 8;
  val |= getUInt8() << 16;
  val |= getUInt8() << 24;
  return val;
}

//!
//! \brief checkCrc Проверить совпадение контрольной суммы
//! \param length   Длина сообщения
//! \return         true когда контрольная сумма совпала
//!
//! length - длина полного сообщения, включая контрольную сумму
bool CsMessageIn::checkCrc(int length) const {
  //Сократить длину до величины тела сообщения (исключаем кс)
  length--;

  int size0 = mBufSize - mStart;
  if (size0 >= length) {
    //Все влезло в первую часть
    int crc0 = (CsMessageOut::crc(mBuffer + mStart, length) | 0x80);
    int crc1 = at(length) & 0xff;
    return crc0 == crc1;
  }
  //Нужно считать по двум частям
  int size1 = length - size0;
  return (CsMessageOut::crc(mBuffer + mStart, size0, mBuffer, size1) | 0x80) ==
         at(length);
}

} // namespace Zubr
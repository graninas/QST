/****************************************************************************
** QST 0.3c beta
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Sql module of the QsT SQL Tools.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: http://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/licenses/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** author (graninas@gmail.com).
**
****************************************************************************/

#include "cyrillic.h"

/*!
	\class Cyrillic
	\brief Класс отвечает за кодировку функции QObject::tr().

	Чтобы выбрать кодировку "UTF-8", достаточно просто создать объект
	класса. Однако, можно выбрать иную кодироку, указав ее в конструкторе.

	\attention Название класса выбрано по недоразумению. В версии QST 0.4 оно
	будет изменено на более подходящее.
*/


/*! Конструктор по умолчанию.

	В качестве параметра можно передать название любого кодека, поддерживаемого
	функцией QObject::tr(). */
Cyrillic::Cyrillic(const QString &codecName)
{
QTextCodec *pTextCodec;
	pTextCodec = QTextCodec::codecForName(codecName.toAscii());
	Q_ASSERT_X(pTextCodec != NULL, "codecPointer", "Invalid Codec");
	QTextCodec::setCodecForTr(pTextCodec);
}

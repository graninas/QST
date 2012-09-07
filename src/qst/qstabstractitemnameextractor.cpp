/****************************************************************************
** QST 0.4.2a release
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst module of the QsT SQL Tools.
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

#include "qstabstractitemnameextractor.h"

namespace Qst
{

/*!
	\class QstAbstractItemNameExtractor
	\brief
	Класс описывает интерфейс для извлечения имени поля из произвольного выражения.

	\inmodule Qst

	QstAbstractItemNameExtractor - абстрактный класс. С помощью его функций
	классы-наследники могут извлекать имя поля из произвольного строкового выражения,
	используя свои алгоритмы.

*/

/*! Конструктор по умолчанию. */
QstAbstractItemNameExtractor::QstAbstractItemNameExtractor()
{
}

/*! Конструктор, принимающий строковую константу, которая содержит имя поля
для извлечения. */
QstAbstractItemNameExtractor::QstAbstractItemNameExtractor(const QString &str)
{
}

/*! Чисто виртуальный деструктор. */
QstAbstractItemNameExtractor::~QstAbstractItemNameExtractor()
{
}

/*! Извлекает имя поля из сохраненной ранее строковой константы.

  Виртуальная функция, возвращает QString(). Может быть переписана в классе-наследнике. */
QString QstAbstractItemNameExtractor::extractItemName() const
{
	return QString();
}

/*! Извлекает имя поля из переданной строковой константы.

  Виртуальная функция, возвращает QString(). Может быть переписана в классе-наследнике. */
QString QstAbstractItemNameExtractor::extractItemName(const QString &str) const
{
	return str;
}


}

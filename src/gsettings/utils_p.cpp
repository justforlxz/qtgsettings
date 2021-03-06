/****************************************************************************
 * This file is part of Qt GSettings.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (C) 2013 Canonical Ltd.
 *
 * $BEGIN_LICENSE:LGPL3$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include "utils_p.h"

namespace Utils {

/*!
 * \brief Converts \p name from 'key-name' notation
 *        to camel case 'keyName' or 'KeyName'.
 * This function converts GSettings key names to names
 * suitable for Qt getters and setters.
 *
 * \param name key name
 * \return key name converted to camel case
 * \internal
 */
QString toCamelCase(const gchar *name)
{
    QString result;
    bool nextUpperCase = false;

    while (*name) {
        if (*name == '-') {
            // Characters following a dash are upper case
            nextUpperCase = true;
        } else if (nextUpperCase) {
            QChar c = QChar::fromLatin1(*name);

            // Leave the dash in front of numbers otherwise it
            // won't be possible to do the inverse
            if (c.isDigit()) {
                result.append(QLatin1Char('-'));
                result.append(c);
            } else {
                result.append(c.toUpper());
            }

            nextUpperCase = false;
        } else {
            result.append(QChar::fromLatin1(*name));
        }

        name++;
    }

    return result;
}

/*!
 * \brief Converts \p name from camel case notation
 *        such as 'keyName' or 'KeyName' to the form 'key-name'.
 * This function converts Qt camel case key names to a
 * format suitable for GSettings.
 *
 * The result of this function must be freed by
 * the caller.
 *
 * \param name key name
 * \return key name converted from camel case
 * \internal
 */
gchar *fromCamelCase(const QString &name)
{
    GString *string = g_string_new(nullptr);

    QByteArray bytes = name.toLatin1();
    for (int i = 0; i < bytes.size(); i++) {
        QChar c = QChar::fromLatin1(bytes.at(i));
        if (c.isUpper()) {
            g_string_append_c(string, '-');
            g_string_append_c(string, c.toLower().toLatin1());
        } else {
            g_string_append_c(string, c.toLatin1());
        }
    }

    return g_string_free(string, false);
}

} // namespace Utils

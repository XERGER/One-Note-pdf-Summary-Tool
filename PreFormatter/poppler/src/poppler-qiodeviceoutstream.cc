/* poppler-qiodevicestream.cc: Qt6 interface to poppler
 * Copyright (C) 2008, Pino Toscano <pino@kde.org>
 * Copyright (C) 2013 Adrian Johnson <ajohnson@redneon.com>
 * Copyright (C) 2020 Albert Astals Cid <aacid@kde.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "poppler-qiodeviceoutstream-private.h"

#include <QtCore/QIODevice>

#include <cstdio>

namespace Poppler {

QIODeviceOutStream::QIODeviceOutStream(QIODevice *device) : m_device(device) { }

QIODeviceOutStream::~QIODeviceOutStream() { }

void QIODeviceOutStream::close() { }

Goffset QIODeviceOutStream::getPos()
{
    return m_device->pos();
}

void QIODeviceOutStream::put(char c)
{
    m_device->putChar(c);
}

static int poppler_qvasprintf(char **buf_ptr, const char *format, va_list ap)
{
    va_list ap_copy;
    va_copy(ap_copy, ap);
    const size_t size = qvsnprintf(nullptr, 0, format, ap_copy) + 1;
    va_end(ap_copy);
    *buf_ptr = new char[size];

    return qvsnprintf(*buf_ptr, size, format, ap);
}

void QIODeviceOutStream::printf(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    char *buf;
    const size_t bufsize = poppler_qvasprintf(&buf, format, ap);
    va_end(ap);
    m_device->write(buf, bufsize);
    delete[] buf;
}

}
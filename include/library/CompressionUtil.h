//
// Created by BlackCyan on 25-6-7.
//

#pragma once
#include <qbytearray.h>
#include <qiodevice.h>

class CompressionUtil {
public:
    static QByteArray compressObject(const std::function<void(QDataStream&)> &serializer);
    static void decompressObject(QByteArray &compressed, const std::function<void(QDataStream &)> &deserializer);
};


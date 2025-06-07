//
// Created by BlackCyan on 25-6-7.
//

#include "CompressionUtil.h"

QByteArray CompressionUtil::compressObject(const std::function<void(QDataStream&)> &serializer) {
    QByteArray rawData;
    QDataStream rawStream(&rawData, QIODevice::WriteOnly);
    serializer(rawStream);
    return qCompress(rawData);
}

void CompressionUtil::decompressObject(QByteArray &compressed, const std::function<void(QDataStream&)> &deserializer) {
    QByteArray rawData = qUncompress(compressed);
    if (rawData.isEmpty()) throw std::runtime_error("Decompression failed");

    QDataStream rawStream(&rawData, QIODevice::ReadOnly);
    deserializer(rawStream);
}

/*
 * Copyright (C) 2018  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>

#include "example.hpp"
#include "cluon-complete.hpp"

int main() {
    testdata::MyTestMessage1 msg1;
    msg1.attribute1(false)
        .attribute2(-10)
        .attribute3(10)
        .attribute4(-11)
        .attribute5(11)
        .attribute6(-12)
        .attribute7(12)
        .attribute8(1.23f)
        .attribute9(1.6)
        .attribute10("Hello World 2")
        .attribute11("Hello World 3");
    // Output values.
    {
        std::stringstream buffer;
        msg1.accept([](int32_t, const std::string &, const std::string &) {},
                    [&buffer](uint32_t, std::string &&, std::string &&n, auto v) { buffer << n << " = " << v << '\n'; },
                    []() {});
        std::cout << buffer.str() << std::endl;
    }

    // Serialize as Protobuf to file.
    cluon::ToProtoVisitor protoEncoder;
    auto encBefore = std::chrono::steady_clock::now();
    {
        msg1.accept(protoEncoder);
    }
    auto encAfter = std::chrono::steady_clock::now();
    std::cout << "Encoding took " << std::chrono::duration <double, std::nano>(encAfter-encBefore).count() << " ns." << std::endl;
    {
        std::fstream fout("libcluon.out", std::ios::binary|std::ios::out|std::ios::trunc);
        fout << protoEncoder.encodedData();
        fout.flush();
        fout.close();
    }

    std::stringstream out;
    out << protoEncoder.encodedData();
    out.flush();

    testdata::MyTestMessage1 msg2;
    // Display default values.
    {
        std::stringstream buffer;
        msg2.accept([](int32_t, const std::string &, const std::string &) {},
                    [&buffer](uint32_t, std::string &&, std::string &&n, auto v) { buffer << n << " = " << v << '\n'; },
                    []() {});
        std::cout << buffer.str() << std::endl;
    }

    // Read back data.
    cluon::FromProtoVisitor protoDecoder;
    {
        auto decBefore = std::chrono::steady_clock::now();
        {
            protoDecoder.decodeFrom<testdata::MyTestMessage1>(out, msg2);
        }
        auto decAfter = std::chrono::steady_clock::now();
        std::cout << "Decoding took " << std::chrono::duration <double, std::nano>(decAfter-decBefore).count() << " ns." << std::endl;
    }
    // Display read values.
    {
        std::stringstream buffer;
        msg2.accept([](int32_t, const std::string &, const std::string &) {},
                    [&buffer](uint32_t, std::string &&, std::string &&n, auto v) { buffer << n << " = " << v << '\n'; },
                    []() {});
        std::cout << buffer.str() << std::endl;
    }

    return 0;
}

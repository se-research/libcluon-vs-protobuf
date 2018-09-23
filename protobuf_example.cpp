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

#include "example.pb.h"

int main() {
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    testdata_MyTestMessage1 msg1;
    msg1.set_attribute1(false);
    msg1.set_attribute2(-10);
    msg1.set_attribute3(10);
    msg1.set_attribute4(-11);
    msg1.set_attribute5(11);
    msg1.set_attribute6(-12);
    msg1.set_attribute7(12);
    msg1.set_attribute8(1.23f);
    msg1.set_attribute9(1.6);
    msg1.set_attribute10("Hello World 2");
    msg1.set_attribute11("Hello World 3");
    // Output values.
    {
        std::cout << msg1.DebugString() << std::endl;
    }

    // Serialize as Protobuf to file.
    {
        std::fstream fout("protobuf.out", std::ios::binary|std::ios::out|std::ios::trunc);
        msg1.SerializeToOstream(&fout);
        fout.flush();
        fout.close();
    }
    std::stringstream out;
    {
        auto encBefore = std::chrono::steady_clock::now();
        {
            msg1.SerializeToOstream(&out);
        }
        auto encAfter = std::chrono::steady_clock::now();
        std::cout << "Encoding took " << std::chrono::duration <double, std::nano>(encAfter-encBefore).count() << " ns." << std::endl;
        out.flush();
    }

    testdata_MyTestMessage1 msg2;
    // Display default values.
    {
        std::cout << msg2.DebugString() << std::endl;
    }

    // Read back data.
    {
        auto decBefore = std::chrono::steady_clock::now();
        {
            msg2.ParseFromIstream(&out);
        }
        auto decAfter = std::chrono::steady_clock::now();
        std::cout << "Decoding took " << std::chrono::duration <double, std::nano>(decAfter-decBefore).count() << " ns." << std::endl;
    }
    // Display read values.
    {
        std::cout << msg2.DebugString() << std::endl;
    }

    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

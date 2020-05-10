//
// Created by linwei on 2020/4/30.
//

#include <iostream>
#include "uav_generated.h"
#include <glog/logging.h>

void Create1() {
    flatbuffers::FlatBufferBuilder builder;
    auto location = cpcm::CreatePos(builder, 11, 299, 50);
    builder.Finish(location);

    std::vector<flatbuffers::Offset<cpcm::Pos> > path{};

    auto msg = cpcm::CreateUav(builder, cpcm::Location_Pos, location.Union(), cpcm::Mode::Mode_Mavic,
                               builder.CreateVector(path), 2477e6, 10e6);

    builder.Finish(msg);

    auto uav = cpcm::GetUav(builder.GetBufferPointer());

    LOG(INFO) << uav->location_as_Pos()->x();
    LOG(INFO) << uav->location_as_Pos()->y();
    LOG(INFO) << uav->location_as_Pos()->z();

}

void Create2() {
    flatbuffers::FlatBufferBuilder builder;
    cpcm::UavBuilder uavBuilder(builder);

    std::vector<flatbuffers::Offset<cpcm::Pos> > path{};

    cpcm::PosBuilder posBuilder(builder);
    posBuilder.add_x(100);
    posBuilder.add_y(200);
    posBuilder.add_z(300);

    uavBuilder.add_freq(2477e6);
    uavBuilder.add_location_type(cpcm::Location_Pos);
    uavBuilder.add_location(
}


int main() {
    Create1();
    return 0;
}
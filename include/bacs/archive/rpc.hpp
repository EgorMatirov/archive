#pragma once

#include <bacs/archive/archive.grpc.pb.h>

#include <grpc++/grpc++.h>

#include <boost/filesystem/path.hpp>

namespace bacs {
namespace archive {
namespace rpc {

void recv_file(grpc::ReaderInterface<Chunk> &reader,
               const boost::filesystem::path &path, utility::Archiver &format);
void send_file(const utility::Archiver &format,
               const boost::filesystem::path &path,
               grpc::WriterInterface<Chunk> &writer);

}  // namespace rpc
}  // namespace archive
}  // namespace bacs

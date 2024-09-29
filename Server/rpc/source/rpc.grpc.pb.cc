// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: rpc.proto

#include "rpc.pb.h"
#include "rpc.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace masterSlaveRPC {

static const char* JobRpc_method_names[] = {
  "/masterSlaveRPC.JobRpc/RequireJob",
  "/masterSlaveRPC.JobRpc/FetchDataFromMap",
  "/masterSlaveRPC.JobRpc/ReportJobStatus",
  "/masterSlaveRPC.JobRpc/ReportEvent",
  "/masterSlaveRPC.JobRpc/HeartBeat",
};

std::unique_ptr< JobRpc::Stub> JobRpc::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< JobRpc::Stub> stub(new JobRpc::Stub(channel, options));
  return stub;
}

JobRpc::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_RequireJob_(JobRpc_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_FetchDataFromMap_(JobRpc_method_names[1], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ReportJobStatus_(JobRpc_method_names[2], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_ReportEvent_(JobRpc_method_names[3], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_HeartBeat_(JobRpc_method_names[4], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status JobRpc::Stub::RequireJob(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::masterSlaveRPC::JobMessage* response) {
  return ::grpc::internal::BlockingUnaryCall< ::masterSlaveRPC::NodeMessage, ::masterSlaveRPC::JobMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_RequireJob_, context, request, response);
}

void JobRpc::Stub::async::RequireJob(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage* request, ::masterSlaveRPC::JobMessage* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::masterSlaveRPC::NodeMessage, ::masterSlaveRPC::JobMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RequireJob_, context, request, response, std::move(f));
}

void JobRpc::Stub::async::RequireJob(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage* request, ::masterSlaveRPC::JobMessage* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_RequireJob_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::masterSlaveRPC::JobMessage>* JobRpc::Stub::PrepareAsyncRequireJobRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::masterSlaveRPC::JobMessage, ::masterSlaveRPC::NodeMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_RequireJob_, context, request);
}

::grpc::ClientAsyncResponseReader< ::masterSlaveRPC::JobMessage>* JobRpc::Stub::AsyncRequireJobRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncRequireJobRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status JobRpc::Stub::FetchDataFromMap(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::masterSlaveRPC::MapDataList* response) {
  return ::grpc::internal::BlockingUnaryCall< ::masterSlaveRPC::NodeMessage, ::masterSlaveRPC::MapDataList, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_FetchDataFromMap_, context, request, response);
}

void JobRpc::Stub::async::FetchDataFromMap(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage* request, ::masterSlaveRPC::MapDataList* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::masterSlaveRPC::NodeMessage, ::masterSlaveRPC::MapDataList, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_FetchDataFromMap_, context, request, response, std::move(f));
}

void JobRpc::Stub::async::FetchDataFromMap(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage* request, ::masterSlaveRPC::MapDataList* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_FetchDataFromMap_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::masterSlaveRPC::MapDataList>* JobRpc::Stub::PrepareAsyncFetchDataFromMapRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::masterSlaveRPC::MapDataList, ::masterSlaveRPC::NodeMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_FetchDataFromMap_, context, request);
}

::grpc::ClientAsyncResponseReader< ::masterSlaveRPC::MapDataList>* JobRpc::Stub::AsyncFetchDataFromMapRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncFetchDataFromMapRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status JobRpc::Stub::ReportJobStatus(::grpc::ClientContext* context, const ::masterSlaveRPC::JobMessage& request, ::google::protobuf::Empty* response) {
  return ::grpc::internal::BlockingUnaryCall< ::masterSlaveRPC::JobMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ReportJobStatus_, context, request, response);
}

void JobRpc::Stub::async::ReportJobStatus(::grpc::ClientContext* context, const ::masterSlaveRPC::JobMessage* request, ::google::protobuf::Empty* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::masterSlaveRPC::JobMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReportJobStatus_, context, request, response, std::move(f));
}

void JobRpc::Stub::async::ReportJobStatus(::grpc::ClientContext* context, const ::masterSlaveRPC::JobMessage* request, ::google::protobuf::Empty* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReportJobStatus_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* JobRpc::Stub::PrepareAsyncReportJobStatusRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::JobMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::protobuf::Empty, ::masterSlaveRPC::JobMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ReportJobStatus_, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* JobRpc::Stub::AsyncReportJobStatusRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::JobMessage& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncReportJobStatusRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status JobRpc::Stub::ReportEvent(::grpc::ClientContext* context, const ::masterSlaveRPC::EventMessage& request, ::google::protobuf::Empty* response) {
  return ::grpc::internal::BlockingUnaryCall< ::masterSlaveRPC::EventMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_ReportEvent_, context, request, response);
}

void JobRpc::Stub::async::ReportEvent(::grpc::ClientContext* context, const ::masterSlaveRPC::EventMessage* request, ::google::protobuf::Empty* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::masterSlaveRPC::EventMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReportEvent_, context, request, response, std::move(f));
}

void JobRpc::Stub::async::ReportEvent(::grpc::ClientContext* context, const ::masterSlaveRPC::EventMessage* request, ::google::protobuf::Empty* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_ReportEvent_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* JobRpc::Stub::PrepareAsyncReportEventRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::EventMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::protobuf::Empty, ::masterSlaveRPC::EventMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_ReportEvent_, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* JobRpc::Stub::AsyncReportEventRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::EventMessage& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncReportEventRaw(context, request, cq);
  result->StartCall();
  return result;
}

::grpc::Status JobRpc::Stub::HeartBeat(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::google::protobuf::Empty* response) {
  return ::grpc::internal::BlockingUnaryCall< ::masterSlaveRPC::NodeMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_HeartBeat_, context, request, response);
}

void JobRpc::Stub::async::HeartBeat(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage* request, ::google::protobuf::Empty* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::masterSlaveRPC::NodeMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_HeartBeat_, context, request, response, std::move(f));
}

void JobRpc::Stub::async::HeartBeat(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage* request, ::google::protobuf::Empty* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_HeartBeat_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* JobRpc::Stub::PrepareAsyncHeartBeatRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::google::protobuf::Empty, ::masterSlaveRPC::NodeMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_HeartBeat_, context, request);
}

::grpc::ClientAsyncResponseReader< ::google::protobuf::Empty>* JobRpc::Stub::AsyncHeartBeatRaw(::grpc::ClientContext* context, const ::masterSlaveRPC::NodeMessage& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncHeartBeatRaw(context, request, cq);
  result->StartCall();
  return result;
}

JobRpc::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      JobRpc_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< JobRpc::Service, ::masterSlaveRPC::NodeMessage, ::masterSlaveRPC::JobMessage, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](JobRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::masterSlaveRPC::NodeMessage* req,
             ::masterSlaveRPC::JobMessage* resp) {
               return service->RequireJob(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      JobRpc_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< JobRpc::Service, ::masterSlaveRPC::NodeMessage, ::masterSlaveRPC::MapDataList, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](JobRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::masterSlaveRPC::NodeMessage* req,
             ::masterSlaveRPC::MapDataList* resp) {
               return service->FetchDataFromMap(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      JobRpc_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< JobRpc::Service, ::masterSlaveRPC::JobMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](JobRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::masterSlaveRPC::JobMessage* req,
             ::google::protobuf::Empty* resp) {
               return service->ReportJobStatus(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      JobRpc_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< JobRpc::Service, ::masterSlaveRPC::EventMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](JobRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::masterSlaveRPC::EventMessage* req,
             ::google::protobuf::Empty* resp) {
               return service->ReportEvent(ctx, req, resp);
             }, this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      JobRpc_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< JobRpc::Service, ::masterSlaveRPC::NodeMessage, ::google::protobuf::Empty, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](JobRpc::Service* service,
             ::grpc::ServerContext* ctx,
             const ::masterSlaveRPC::NodeMessage* req,
             ::google::protobuf::Empty* resp) {
               return service->HeartBeat(ctx, req, resp);
             }, this)));
}

JobRpc::Service::~Service() {
}

::grpc::Status JobRpc::Service::RequireJob(::grpc::ServerContext* context, const ::masterSlaveRPC::NodeMessage* request, ::masterSlaveRPC::JobMessage* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status JobRpc::Service::FetchDataFromMap(::grpc::ServerContext* context, const ::masterSlaveRPC::NodeMessage* request, ::masterSlaveRPC::MapDataList* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status JobRpc::Service::ReportJobStatus(::grpc::ServerContext* context, const ::masterSlaveRPC::JobMessage* request, ::google::protobuf::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status JobRpc::Service::ReportEvent(::grpc::ServerContext* context, const ::masterSlaveRPC::EventMessage* request, ::google::protobuf::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status JobRpc::Service::HeartBeat(::grpc::ServerContext* context, const ::masterSlaveRPC::NodeMessage* request, ::google::protobuf::Empty* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace masterSlaveRPC


/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#ifdef FUSION_CONVADDRELU_OP

#pragma once

#include "framework/operator.h"
#include "framework/program/program-optimize/fusion_op_register.h"
#include "operators/kernel/conv_add_relu_kernel.h"
#include "operators/op_param.h"

namespace paddle_mobile {
namespace operators {

class FusionConvAddReluOpMatcher : public framework::FusionOpMatcher {
 public:
  FusionConvAddReluOpMatcher() {
    node_ = framework::Node(G_OP_TYPE_CONV);
    node_ > std::make_shared<framework::Node>(G_OP_TYPE_ELEMENTWISE_ADD) >
        std::make_shared<framework::Node>(G_OP_TYPE_RELU);
  }

  void FolderNodes(
      framework::Node *node,
      std::vector<std::shared_ptr<framework::Node>> *removed_nodes) {
    node->Folder(node_.Depth(), Type(),
                 {{G_OP_TYPE_ELEMENTWISE_ADD, {{"Y", "Y"}}}}, removed_nodes);
  }
  std::string Type() { return G_OP_TYPE_FUSION_CONV_ADD_RELU; }
};

template <typename DeviceType, typename T>
class FusionConvAddReluOp : public framework::OperatorWithKernel<
                                DeviceType, FusionConvAddReluParam,
                                operators::ConvAddReluKernel<DeviceType, T>> {
 public:
  FusionConvAddReluOp(const string &type, const VariableNameMap &inputs,
                      const VariableNameMap &outputs,
                      const framework::AttributeMap &attrs,
                      std::shared_ptr<framework::Scope> scope)
      : framework::OperatorWithKernel<
            DeviceType, FusionConvAddReluParam,
            operators::ConvAddReluKernel<DeviceType, T>>(type, inputs, outputs,
                                                         attrs, scope) {}

  using framework::OperatorWithKernel<
      DeviceType, FusionConvAddReluParam,
      operators::ConvAddReluKernel<DeviceType, T>>::OperatorWithKernel;
  void InferShape() const override;

 protected:
};

#ifdef PADDLE_MOBILE_CPU

//#ifndef CONV_ADD_RELU_REGISTER
//#define CONV_ADD_RELU_REGISTER
// static framework::FusionOpRegistrar fusion_conv_add_relu_registrar(new
// FusionConvAddReluOpMatcher());
//#endif

#endif
#ifdef PADDLE_MOBILE_MALI_GPU
#endif
#ifdef PADDLE_MOBILE_FPGA
#endif

}  // namespace operators
}  // namespace paddle_mobile

#ifdef PADDLE_MOBILE_CPU
USE_OP_CPU(fusion_conv_add_relu);
#endif
#ifdef PADDLE_MOBILE_MALI_GPU
#endif
#ifdef PADDLE_MOBILE_FPGA
#endif

#endif

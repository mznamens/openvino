// Copyright (C) 2018-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include "transformations/convert_opset3_to_opset2/convert_broadcast3.hpp"

#include <memory>
#include <vector>

#include <ngraph/rt_info.hpp>

#include <ngraph/opsets/opset1.hpp>
#include <ngraph/opsets/opset3.hpp>

void ngraph::pass::ConvertBroadcast3::convert_broadcast3() {
    auto weights = std::make_shared<pattern::op::Label>(element::f32, Shape {1});
    auto shp = std::make_shared<pattern::op::Label>(element::i64, Shape {1});
    auto axes = std::make_shared<pattern::op::Label>(element::i64, Shape {1});
    auto broadcast = std::make_shared<ngraph::opset3::Broadcast>(weights, shp, axes);

    auto broadcast_no_axes = std::make_shared<ngraph::opset3::Broadcast>(weights, shp);

    ngraph::graph_rewrite_callback callback = [](pattern::Matcher& m) {
        auto broadcast = std::dynamic_pointer_cast<ngraph::opset3::Broadcast>(m.get_match_root());
        if (!broadcast) {
            return false;
        }

        auto input = broadcast->input_value(0);
        auto target_shape = broadcast->input_value(1);

        auto last_node = input.get_node_shared_ptr();
        auto broadcast_type = broadcast->get_broadcast_spec();

        if (broadcast_type == op::BroadcastType::NUMPY) {
            last_node = std::make_shared<ngraph::opset1::Broadcast>(input, target_shape, op::AutoBroadcastType::NUMPY);
            ngraph::copy_runtime_info(broadcast, last_node);
        } else if (broadcast_type == op::BroadcastType::PDPD) {
            last_node = std::make_shared<ngraph::opset1::Broadcast>(input, target_shape, op::AutoBroadcastType::PDPD);
            ngraph::copy_runtime_info(broadcast, last_node);
        } else if (broadcast_type == op::BroadcastType::NONE) {
            last_node = std::make_shared<ngraph::opset1::Broadcast>(input, target_shape, broadcast->input_value(2), op::AutoBroadcastType::NONE);
            ngraph::copy_runtime_info(broadcast, last_node);
        } else if (broadcast_type == op::BroadcastType::BIDIRECTIONAL) {
            auto constant_one = std::make_shared<ngraph::opset1::Constant>(input.get_element_type(), Shape({1}), std::vector<int>{1});
            auto broadcast_ones = std::make_shared<ngraph::opset1::Broadcast>(constant_one, target_shape, op::AutoBroadcastType::NUMPY);
            last_node = std::make_shared<ngraph::opset1::Multiply>(input, broadcast_ones);
            ngraph::copy_runtime_info(broadcast, {last_node, broadcast_ones, constant_one});
        }

        last_node->set_friendly_name(broadcast->get_friendly_name());

        ngraph::replace_node(m.get_match_root(), last_node);
        return true;
    };

    auto m = std::make_shared<ngraph::pattern::Matcher>(broadcast, "ConvertBroadcast3");
    auto m_no_axes = std::make_shared<ngraph::pattern::Matcher>(broadcast_no_axes, "ConvertBroadcast3NoAxes");
    this->add_matcher(m, callback, PassProperty::CHANGE_DYNAMIC_STATE);
    this->add_matcher(m_no_axes, callback, PassProperty::CHANGE_DYNAMIC_STATE);
}

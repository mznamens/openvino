// Copyright (C) 2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <vector>

#include "low_precision_transformations/concat_with_intermediate_transformation.hpp"
#include "common_test_utils/test_constants.hpp"

using namespace LayerTestsDefinitions;
using namespace InferenceEngine::details;

namespace {
const std::vector<InferenceEngine::Precision> netPrecisions = {
        InferenceEngine::Precision::FP32,
        InferenceEngine::Precision::FP16
};

const std::vector<LayerTransformation::Params> trasformationParamValues = {
    LayerTestsUtils::LayerTransformationParamsFactory::createParams().setUpdatePrecisions(true),
    LayerTestsUtils::LayerTransformationParamsFactory::createParams().setUpdatePrecisions(false),
    LayerTestsUtils::LayerTransformationParamsFactory::createParamsI8I8(),
    LayerTestsUtils::LayerTransformationParamsFactory::createParamsU8I8()
};

const std::vector<bool> transparentIntermediateValues = { true, false };
const std::vector<bool> multiChannelValues = { /*true,*/ false };

INSTANTIATE_TEST_CASE_P(LPT, ConcatWithIntermediateTransformation,
    ::testing::Combine(
        ::testing::ValuesIn(netPrecisions),
        ::testing::Values(InferenceEngine::SizeVector({ 1, 3, 16, 16 })),
        ::testing::Values(CommonTestUtils::DEVICE_CPU),
        ::testing::ValuesIn(trasformationParamValues),
        ::testing::ValuesIn(transparentIntermediateValues),
        ::testing::ValuesIn(multiChannelValues)),
    ConcatWithIntermediateTransformation::getTestCaseName);
}  // namespace

//@HEADER
// ************************************************************************
//
//                        Kokkos v. 4.0
//       Copyright (2022) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Part of Kokkos, under the Apache License v2.0 with LLVM Exceptions.
// See https://kokkos.org/LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//@HEADER
#include <experimental/mdspan>
#include <vector>

#include <gtest/gtest.h>

#include <utility>

namespace stdex = std::experimental;
_MDSPAN_INLINE_VARIABLE constexpr auto dyn = stdex::dynamic_extent;

TEST(TestSubmdspanLayoutRightStaticSizedRankReducing3Dto1D, test_submdspan_layout_right_static_sized_rank_reducing_3d_to_1d) {
  std::vector<int> d(2 * 3 * 4, 0);
  stdex::mdspan<int, stdex::extents<size_t,2, 3, 4>> m(d.data());
  __MDSPAN_OP(m, 1, 1, 1) = 42;
  auto sub0 = stdex::submdspan(m, 1, 1, stdex::full_extent);
  static_assert(decltype(sub0)::rank()==1,"unexpected submdspan rank");
  ASSERT_EQ(sub0.rank(),         1);
  ASSERT_EQ(sub0.rank_dynamic(), 0);
  ASSERT_EQ(sub0.extent(0),      4);
  ASSERT_EQ((__MDSPAN_OP(sub0, 1)), 42);
}

TEST(TestSubmdspanLayoutLeftStaticSizedRankReducing3Dto1D, test_submdspan_layout_left_static_sized_rank_reducing_3d_to_1d) {
  std::vector<int> d(2 * 3 * 4, 0);
  stdex::mdspan<int, stdex::extents<size_t,2, 3, 4>, stdex::layout_left> m(d.data());
  __MDSPAN_OP(m, 1, 1, 1) = 42;
  auto sub0 = stdex::submdspan(m, 1, 1, stdex::full_extent);
  ASSERT_EQ(sub0.rank(),         1);
  ASSERT_EQ(sub0.rank_dynamic(), 0);
  ASSERT_EQ(sub0.extent(0),      4);
  ASSERT_EQ((__MDSPAN_OP(sub0, 1)), 42);
}

TEST(TestSubmdspanLayoutRightStaticSizedRankReducingNested3Dto0D, test_submdspan_layout_right_static_sized_rank_reducing_nested_3d_to_0d) {
  std::vector<int> d(2 * 3 * 4, 0);
  stdex::mdspan<int, stdex::extents<size_t,2, 3, 4>> m(d.data());
  __MDSPAN_OP(m, 1, 1, 1) = 42;
  auto sub0 = stdex::submdspan(m, 1, stdex::full_extent, stdex::full_extent);
  ASSERT_EQ(sub0.rank(),         2);
  ASSERT_EQ(sub0.rank_dynamic(), 0);
  ASSERT_EQ(sub0.extent(0),      3);
  ASSERT_EQ(sub0.extent(1),      4);
  ASSERT_EQ((__MDSPAN_OP(sub0, 1, 1)), 42);
  auto sub1 = stdex::submdspan(sub0, 1, stdex::full_extent);
  ASSERT_EQ(sub1.rank(),         1);
  ASSERT_EQ(sub1.rank_dynamic(), 0);
  ASSERT_EQ(sub1.extent(0),      4);
  ASSERT_EQ((__MDSPAN_OP(sub1,1)),42);
  auto sub2 = stdex::submdspan(sub1, 1);
  ASSERT_EQ(sub2.rank(),         0);
  ASSERT_EQ(sub2.rank_dynamic(), 0);
  ASSERT_EQ((__MDSPAN_OP0(sub2)), 42);
}

TEST(TestSubmdspanLayoutRightStaticSizedPairs, test_submdspan_layout_right_static_sized_pairs) {
  std::vector<int> d(2 * 3 * 4, 0);
  stdex::mdspan<int, stdex::extents<size_t,2, 3, 4>> m(d.data());
  __MDSPAN_OP(m, 1, 1, 1) = 42;
  auto sub0 = stdex::submdspan(m, std::pair<int,int>{1, 2}, std::pair<int,int>{1, 3}, std::pair<int,int>{1, 4});
  ASSERT_EQ(sub0.rank(),         3);
  ASSERT_EQ(sub0.rank_dynamic(), 3);
  ASSERT_EQ(sub0.extent(0),      1);
  ASSERT_EQ(sub0.extent(1),      2);
  ASSERT_EQ(sub0.extent(2),      3);
  ASSERT_EQ((__MDSPAN_OP(sub0, 0, 0, 0)), 42);
}

TEST(TestSubmdspanLayoutRightStaticSizedTuples, test_submdspan_layout_right_static_sized_tuples) {
  std::vector<int> d(2 * 3 * 4, 0);
  stdex::mdspan<int, stdex::extents<size_t,2, 3, 4>> m(d.data());
  __MDSPAN_OP(m, 1, 1, 1) = 42;
  auto sub0 = stdex::submdspan(m, std::tuple<int,int>{1, 2}, std::tuple<int,int>{1, 3}, std::tuple<int,int>{1, 4});
  ASSERT_EQ(sub0.rank(),         3);
  ASSERT_EQ(sub0.rank_dynamic(), 3);
  ASSERT_EQ(sub0.extent(0),      1);
  ASSERT_EQ(sub0.extent(1),      2);
  ASSERT_EQ(sub0.extent(2),      3);
  ASSERT_EQ((__MDSPAN_OP(sub0, 0, 0, 0)),       42);
}


//template<class LayoutOrg, class LayoutSub, class ExtentsOrg, class ExtentsSub, class ... SubArgs>


template<size_t ... Args>
using args_t = std::index_sequence<Args...>;

using submdspan_test_types =
  ::testing::Types<
      // LayoutLeft to LayoutLeft
      std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,1>, args_t<10>,          stdex::dextents<size_t,1>, stdex::full_extent_t>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,1>, args_t<10>,          stdex::dextents<size_t,1>, std::pair<int,int>>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,1>, args_t<10>,          stdex::dextents<size_t,0>, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,2>, args_t<10,20>,       stdex::dextents<size_t,2>, stdex::full_extent_t, stdex::full_extent_t>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,2>, args_t<10,20>,       stdex::dextents<size_t,2>, stdex::full_extent_t, std::pair<int,int>>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,2>, args_t<10,20>,       stdex::dextents<size_t,1>, stdex::full_extent_t, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,3>, args_t<10,20,30>,    stdex::dextents<size_t,3>, stdex::full_extent_t, stdex::full_extent_t, std::pair<int,int>>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,3>, args_t<10,20,30>,    stdex::dextents<size_t,2>, stdex::full_extent_t, std::pair<int,int>, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,3>, args_t<10,20,30>,    stdex::dextents<size_t,1>, stdex::full_extent_t, int, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,3>, args_t<10,20,30>,    stdex::dextents<size_t,1>, std::pair<int,int>, int, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,6>, args_t<6,4,5,6,7,8>, stdex::dextents<size_t,3>, stdex::full_extent_t, stdex::full_extent_t, std::pair<int,int>, int, int, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,6>, args_t<6,4,5,6,7,8>, stdex::dextents<size_t,2>, stdex::full_extent_t, std::pair<int,int>, int, int, int, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,6>, args_t<6,4,5,6,7,8>, stdex::dextents<size_t,1>, stdex::full_extent_t, int, int, int ,int, int>
    , std::tuple<stdex::layout_left, stdex::layout_left, stdex::dextents<size_t,6>, args_t<6,4,5,6,7,8>, stdex::dextents<size_t,1>, std::pair<int,int>, int, int, int, int, int>
    // LayoutRight to LayoutRight
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,1>, args_t<10>,          stdex::dextents<size_t,1>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,1>, args_t<10>,          stdex::dextents<size_t,1>, std::pair<int,int>>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,1>, args_t<10>,          stdex::dextents<size_t,0>, int>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,2>, args_t<10,20>,       stdex::dextents<size_t,2>, stdex::full_extent_t, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,2>, args_t<10,20>,       stdex::dextents<size_t,2>, std::pair<int,int>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,2>, args_t<10,20>,       stdex::dextents<size_t,1>, int, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,3>, args_t<10,20,30>,    stdex::dextents<size_t,3>, std::pair<int,int>, stdex::full_extent_t, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,3>, args_t<10,20,30>,    stdex::dextents<size_t,2>, int, std::pair<int,int>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,3>, args_t<10,20,30>,    stdex::dextents<size_t,1>, int, int, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,6>, args_t<6,4,5,6,7,8>, stdex::dextents<size_t,3>, int, int, int, std::pair<int,int>, stdex::full_extent_t, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,6>, args_t<6,4,5,6,7,8>, stdex::dextents<size_t,2>, int, int, int, int, std::pair<int,int>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::dextents<size_t,6>, args_t<6,4,5,6,7,8>, stdex::dextents<size_t,1>, int, int, int, int, int, stdex::full_extent_t>
    // LayoutRight to LayoutRight Check Extents Preservation
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10>,           args_t<10>,          stdex::extents<size_t,10>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10>,           args_t<10>,          stdex::extents<size_t,dyn>, std::pair<int,int>>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10>,           args_t<10>,          stdex::extents<size_t>, int>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10,20>,        args_t<10,20>,       stdex::extents<size_t,10,20>, stdex::full_extent_t, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10,20>,        args_t<10,20>,       stdex::extents<size_t,dyn,20>, std::pair<int,int>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10,20>,        args_t<10,20>,       stdex::extents<size_t,20>, int, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10,20,30>,     args_t<10,20,30>,    stdex::extents<size_t,dyn,20,30>, std::pair<int,int>, stdex::full_extent_t, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10,20,30>,     args_t<10,20,30>,    stdex::extents<size_t,dyn,30>, int, std::pair<int,int>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,10,20,30>,     args_t<10,20,30>,    stdex::extents<size_t,30>, int, int, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,6,4,5,6,7,8>,  args_t<6,4,5,6,7,8>, stdex::extents<size_t,dyn,7,8>, int, int, int, std::pair<int,int>, stdex::full_extent_t, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,6,4,5,6,7,8>,  args_t<6,4,5,6,7,8>, stdex::extents<size_t,dyn,8>, int, int, int, int, std::pair<int,int>, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_right, stdex::extents<size_t,6,4,5,6,7,8>,  args_t<6,4,5,6,7,8>, stdex::extents<size_t,8>, int, int, int, int, int, stdex::full_extent_t>
    // LayoutLeft to LayoutStride
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::dextents<size_t,1>,          args_t<10>,          stdex::dextents<size_t,1>, stdex::strided_index_range<int,int,int>>
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::dextents<size_t,2>,          args_t<10,20>,       stdex::dextents<size_t,1>, stdex::strided_index_range<int,int,int>, int>
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::dextents<size_t,2>,          args_t<10,20>,       stdex::dextents<size_t,2>, std::pair<int,int>, stdex::full_extent_t>
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::dextents<size_t,2>,          args_t<10,20>,       stdex::dextents<size_t,2>, std::pair<int,int>, stdex::strided_index_range<int,int,int>>
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::dextents<size_t,2>,          args_t<10,20>,       stdex::dextents<size_t,2>, std::pair<int,int>, stdex::strided_index_range<int,int,int>>
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::dextents<size_t,2>,          args_t<10,20>,       stdex::dextents<size_t,2>, stdex::strided_index_range<int,int,int>, stdex::strided_index_range<int,int,int>>
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::extents<size_t,6,4,5,6,7,8>, args_t<6,4,5,6,7,8>, stdex::extents<size_t,6,dyn,8>, stdex::full_extent_t, int, std::pair<int,int>, int, int, stdex::full_extent_t>
    , std::tuple<stdex::layout_left, stdex::layout_stride,  stdex::extents<size_t,6,4,5,6,7,8>, args_t<6,4,5,6,7,8>, stdex::extents<size_t,4,dyn,7>, int, stdex::full_extent_t, std::pair<int,int>, int, stdex::full_extent_t, int>
    // layout_right to layout_stride
    , std::tuple<stdex::layout_right, stdex::layout_stride, stdex::extents<size_t,6,4,5,6,7,8>, args_t<6,4,5,6,7,8>, stdex::extents<size_t,6,dyn,8>, stdex::full_extent_t, int, std::pair<int,int>, int, int, stdex::full_extent_t>
    , std::tuple<stdex::layout_right, stdex::layout_stride, stdex::extents<size_t,6,4,5,6,7,8>, args_t<6,4,5,6,7,8>, stdex::extents<size_t,4,dyn,7>, int, stdex::full_extent_t, std::pair<int,int>, int, stdex::full_extent_t, int>
    >;

template<class T> struct TestSubMDSpan;

template<class LayoutOrg, class LayoutSub, class ExtentsOrg, class ExtentsSub, class ... SubArgs, size_t ... ConstrArgs>
struct TestSubMDSpan<
  std::tuple<LayoutOrg,
             LayoutSub,
             ExtentsOrg,
	     std::index_sequence<ConstrArgs...>,
             ExtentsSub,
             SubArgs...>>
  : public ::testing::Test {

  using mds_org_t = stdex::mdspan<int, ExtentsOrg, LayoutOrg>;
  using mds_sub_t = stdex::mdspan<int, ExtentsSub, LayoutSub>;
  using map_t = typename mds_org_t::mapping_type;

  using mds_sub_deduced_t = decltype(stdex::submdspan(mds_org_t(nullptr, map_t()), SubArgs()...));
  using sub_args_t = std::tuple<SubArgs...>;

  static int create_slice_arg(int) {
    return 2;
  }
  static std::pair<int,int> create_slice_arg(std::pair<int,int>) {
    return std::pair<int,int>(1,3);
  }
  static stdex::strided_index_range<int,int,int> create_slice_arg(stdex::strided_index_range<int,int,int>) {
    return stdex::strided_index_range<int,int,int>(1,3,2);
  }
  static stdex::full_extent_t create_slice_arg(stdex::full_extent_t) {
    return stdex::full_extent;
  }

  template<class SrcExtents, class SubExtents, class ... SliceArgs>
  static bool match_expected_extents(int src_idx, int sub_idx, SrcExtents src_ext, SubExtents sub_ext, int, SliceArgs ... slices) {
    return match_expected_extents(++src_idx, sub_idx, src_ext, sub_ext, slices...);
  }
  template<class SrcExtents, class SubExtents, class ... SliceArgs>
  static bool match_expected_extents(int src_idx, int sub_idx, SrcExtents src_ext, SubExtents sub_ext, std::pair<int,int> p, SliceArgs ... slices) {
    return (sub_ext.extent(sub_idx)==p.second-p.first) && match_expected_extents(++src_idx, ++sub_idx, src_ext, sub_ext, slices...);
  }
  template<class SrcExtents, class SubExtents, class ... SliceArgs>
  static bool match_expected_extents(int src_idx, int sub_idx, SrcExtents src_ext, SubExtents sub_ext, stdex::strided_index_range<int,int,int> p, SliceArgs ... slices) {
    return (sub_ext.extent(sub_idx)==(p.extent+p.stride-1)/p.stride) && match_expected_extents(++src_idx, ++sub_idx, src_ext, sub_ext, slices...);
  }
  template<class SrcExtents, class SubExtents, class ... SliceArgs>
  static bool match_expected_extents(int src_idx, int sub_idx, SrcExtents src_ext, SubExtents sub_ext, stdex::full_extent_t, SliceArgs ... slices) {
    return (sub_ext.extent(sub_idx)==src_ext.extent(src_idx)) && match_expected_extents(++src_idx, ++sub_idx, src_ext, sub_ext, slices...);
  }
  template<class SrcExtents, class SubExtents>
  static bool match_expected_extents(int, int, SrcExtents, SubExtents) { return true; }

  static void run() {
    typename mds_org_t::mapping_type map(typename mds_org_t::extents_type(ConstrArgs...));
    int data[25000];
    mds_org_t src(data, map);
    auto sub = stdex::submdspan(src, create_slice_arg(SubArgs())...);
    bool match = match_expected_extents(0, 0, src.extents(), sub.extents(), create_slice_arg(SubArgs())...);
    EXPECT_TRUE(match);
  }

};


TYPED_TEST_SUITE(TestSubMDSpan, submdspan_test_types);

TYPED_TEST(TestSubMDSpan, submdspan_return_type) {
  static_assert(std::is_same<typename TestFixture::mds_sub_t,
                             typename TestFixture::mds_sub_deduced_t>::value,
                "SubMDSpan: wrong return type");
  TestFixture::run();
}

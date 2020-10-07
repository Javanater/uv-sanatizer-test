#include <limits>
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

#include "../uv-sanatizer/debounce_task.hpp"
#include <limits>

BOOST_AUTO_TEST_SUITE(debounce_suite)

BOOST_AUTO_TEST_CASE(reject_active) {
  auto const debounce_time = 10;
  debounce_task_t debounce(debounce_time);

  unsigned long time = 0;
  BOOST_TEST(debounce(false, time) == false);
  BOOST_TEST(debounce(true, time) == false);
  BOOST_TEST(debounce(true, debounce_time - 1) == false);
  BOOST_TEST(debounce(true, debounce_time) == true);
}

BOOST_AUTO_TEST_CASE(coverage) {
  auto const debounce_time = 10;
  debounce_task_t debounce(debounce_time);

  BOOST_TEST(debounce.get_state() == debounce_task_t::INACTIVE);

  BOOST_TEST(debounce(false, 0) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::INACTIVE);

  BOOST_TEST(debounce(true, 1) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_INACTIVE);

  BOOST_TEST(debounce(false, 2) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::INACTIVE);

  BOOST_TEST(debounce(true, 3) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_INACTIVE);

  BOOST_TEST(debounce(true, 12) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_INACTIVE);

  BOOST_TEST(debounce(true, 13) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::ACTIVE);

  BOOST_TEST(debounce(true, 14) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::ACTIVE);

  BOOST_TEST(debounce(false, 15) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_ACTIVE);

  BOOST_TEST(debounce(true, 16) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::ACTIVE);

  BOOST_TEST(debounce(false, 17) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_ACTIVE);

  BOOST_TEST(debounce(false, 26) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_ACTIVE);

  BOOST_TEST(debounce(false, 27) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::INACTIVE);

  BOOST_TEST(debounce(true, 28) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_INACTIVE);

  BOOST_TEST(debounce(false, 29) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::INACTIVE);

  BOOST_TEST(debounce(true, 30) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_INACTIVE);

  BOOST_TEST(debounce(true, 39) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_INACTIVE);

  BOOST_TEST(debounce(true, 40) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::ACTIVE);

  BOOST_TEST(debounce(true, 41) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::ACTIVE);

  BOOST_TEST(debounce(false, 42) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_ACTIVE);

  BOOST_TEST(debounce(true, 43) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::ACTIVE);

  BOOST_TEST(debounce(false, 44) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_ACTIVE);

  BOOST_TEST(debounce(false, 53) == true);
  BOOST_TEST(debounce.get_state() == debounce_task_t::DEBOUCE_ACTIVE);

  BOOST_TEST(debounce(false, 54) == false);
  BOOST_TEST(debounce.get_state() == debounce_task_t::INACTIVE);
}

BOOST_AUTO_TEST_SUITE_END()

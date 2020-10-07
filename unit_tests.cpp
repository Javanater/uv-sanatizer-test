#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

#include "../uv-sanatizer/debounce_task.hpp"
#include "../uv-sanatizer/sanatize_task.hpp"

BOOST_AUTO_TEST_SUITE(uv_suite)

BOOST_AUTO_TEST_CASE(debounce_coverage) {
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

BOOST_AUTO_TEST_CASE(sanatize_coverage) {
  long long const sanatize_time = 10;
  long long const cool_down_time = 5;
  sanatize_task_t sanatize(sanatize_time, cool_down_time);

  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);

  BOOST_TEST(sanatize(false, 0) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);

  BOOST_TEST(sanatize(true, 1) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE_WAIT_FOR_RELEASE);

  BOOST_TEST(sanatize(true, 2) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE_WAIT_FOR_RELEASE);

  BOOST_TEST(sanatize(false, 3) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  BOOST_TEST(sanatize(false, 4) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  BOOST_TEST(sanatize(true, 5) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::CANCEL_WAIT_FOR_RELEASE);

  BOOST_TEST(sanatize(true, 6) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::CANCEL_WAIT_FOR_RELEASE);

  BOOST_TEST(sanatize(false, 7) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(true, 8) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(false, 9) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(false, 11) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(false, 12) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);
}

BOOST_AUTO_TEST_SUITE_END()
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include <limits>

#include "../uv-sanatizer/debounce_task.hpp"
#include "../uv-sanatizer/rising_edge_task.hpp"
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
  duration_t const sanatize_time = 10;
  duration_t const cool_down_time = 5;
  sanatize_task_t sanatize(sanatize_time, cool_down_time);

  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);

  BOOST_TEST(sanatize(false, 0) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);

  BOOST_TEST(sanatize(true, 1) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  BOOST_TEST(sanatize(false, 2) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  BOOST_TEST(sanatize(true, 3) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(true, 4) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(false, 5) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(false, 7) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  BOOST_TEST(sanatize(false, 8) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);
}

BOOST_AUTO_TEST_CASE(sanatize_overflow) {
  duration_t const sanatize_time = 10;
  duration_t const cool_down_time = 5;
  sanatize_task_t sanatize(sanatize_time, cool_down_time);

  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);

  auto current_time = std::numeric_limits<timepoint_t>::max(); // 255
  BOOST_TEST(sanatize(true, current_time) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  current_time += 1; // 0; 0 - 255 = 1
  BOOST_TEST(sanatize(false, current_time) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  current_time += sanatize_time - 2; // 8; 8 - 255 = 9
  BOOST_TEST(sanatize(false, current_time) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  current_time += 1; // 9; 9 - 255 = 10
  BOOST_TEST(sanatize(false, current_time) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);
}

BOOST_AUTO_TEST_CASE(cool_down_overflow) {
  duration_t const sanatize_time = 10;
  duration_t const cool_down_time = 5;
  sanatize_task_t sanatize(sanatize_time, cool_down_time);

  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);

  auto current_time =
      std::numeric_limits<timepoint_t>::max() - sanatize_time; // 255 - 10 = 245
  BOOST_TEST(sanatize(true, current_time) == true);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::SANATIZE);

  current_time += sanatize_time; // 255
  BOOST_TEST(sanatize(false, current_time) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  current_time += 1; // 0; 0 - 255 = 1
  BOOST_TEST(sanatize(false, current_time) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::COOL_DOWN);

  current_time += cool_down_time - 1; // 4; 4 - 255 = 5
  BOOST_TEST(sanatize(false, current_time) == false);
  BOOST_TEST(sanatize.get_state() == sanatize_task_t::IDLE);
}

BOOST_AUTO_TEST_CASE(rising_edge_coverage) {
  {
    rising_edge_task_t rising_edge;
    BOOST_TEST(rising_edge(false) == false);
    BOOST_TEST(rising_edge(false) == false);
    BOOST_TEST(rising_edge(true) == true);
    BOOST_TEST(rising_edge(true) == false);
    BOOST_TEST(rising_edge(false) == false);
    BOOST_TEST(rising_edge(true) == true);
  }

  {
    rising_edge_task_t rising_edge;
    BOOST_TEST(rising_edge(true) == true);
    BOOST_TEST(rising_edge(true) == false);
  }
}

BOOST_AUTO_TEST_SUITE_END()
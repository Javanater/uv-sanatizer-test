#include "../uv-sanatizer/debounce_task.hpp"
#include <array>
#include <bits/stdint-uintn.h>
#include <boost/optional.hpp>
#include <iterator>
#include <utility>
#include <vector>

struct params {
  bool active;
  unsigned long time;
};

union params_uint8_arr {
  params p;
  std::array<uint8_t, sizeof(params)> a;
};

union long_long_arr {
  long long l;
  std::array<uint8_t, sizeof(long long)> a;
};

auto decode_input(const uint8_t *Data, size_t Size) {
  if (Size < sizeof(long long))
    return std::make_tuple(boost::optional<long long>{}, std::vector<params>{});

  long_long_arr lla;

  for (auto i = 0; i < sizeof(long long); ++i) {
    lla.a[i] = Data[i];
  }

  Size -= sizeof(long long);
  Data += sizeof(long long);

  std::vector<params> params_vec;

  for (; Size >= sizeof(params);) {
    params_uint8_arr pua;

    for (auto i = 0; i < sizeof(params); ++i) {
      pua.a[i] = Data[i];
    }

    params_vec.push_back(pua.p);
    Size -= sizeof(params);
    Data += sizeof(params);
  }

  return std::make_tuple(boost::optional<long long>{lla.l}, params_vec);
}

// Fuzzer that attempts to invoke undefined behavior for signed integer overflow
// cppcheck-suppress unusedFunction symbolName=LLVMFuzzerTestOneInput
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  auto [debounce_time, params_list] = decode_input(Data, Size);

  if (!debounce_time)
    return 0;

  debounce_task_t debounce(*debounce_time);

  for (auto const &params : params_list) {
    debounce(params.active, params.time);
  }

  return 0;
}

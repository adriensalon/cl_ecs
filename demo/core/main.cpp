#include <compute/core/buffer.hpp>
#include <compute/core/context.hpp>
#include <compute/core/device.hpp>
#include <compute/core/kernel.hpp>

#include <iostream>

std::string add_kernel = R"(
    kernel void addition(__global float* a, __global float* b)
    {
        int x = get_global_id(0);
        b[x] = b[x] + a[x];
    } 
)";

void print(const std::vector<float>& vec)
{
    for (int _k = 0; _k < vec.size(); _k++) {
        std::cout << vec[_k] << " ";
    }
}

int main()
{
    auto _dev = compute::device::get_device(0);
    auto _ctx = compute::context(_dev);
    std::cout << "device name : " << _dev.get_name() << std::endl;

    auto _abuf = compute::array_buffer<float>(_ctx, 9);
    auto _bbuf = compute::array_buffer<float>(_ctx, 9);
    _abuf.set(std::vector<float>(9, 0.22f));
    _bbuf.set(std::vector<float>(9, 0.44f));
    print(_abuf.fetch().get());
    std::cout << "+" << std::endl;
    print(_bbuf.fetch().get());
    std::cout << "=" << std::endl;

    auto _krn = compute::kernel(_ctx, add_kernel, "addition");
    _krn.set_arg<float>(0, _abuf);
    _krn.set_arg<float>(1, _bbuf);
    _krn.run({ 9 });
    print(_bbuf.fetch().get());

    return 0;
}
#include"../../include/fast_io.h"

/*
Thanks Jsoniter Scala author for providing test suite
https://github.com/plokhotnyuk/jsoniter-scala/blob/e8e246d6b716413d9fe45fc57bf214aaca3d843b/jsoniter-scala-core/shared/src/test/scala/com/github/plokhotnyuk/jsoniter_scala/core/JsonWriterSpec.scala#L488-L692
*/

template<std::floating_point F>
inline void check(F f)
{
	println(f);
}

int main()
{
	check(0.0f);
	check(-0.0f);
	check(1.0f);
	check(-1.0f);
	check(1.0E7f);
	check(fast_io::bit_cast<float>(static_cast<std::uint32_t>(0x00800000))); // subnormal
	check(9999999.0f);
	check(0.001f);
	check(0.0009999999f);
	check(std::numeric_limits<float>::min());
	check(std::numeric_limits<float>::lowest());
	check(std::numeric_limits<float>::max());
	check(3.3554448E7f);
	check(8.999999E9f);
	check(3.4366717E10f);
	check(4.7223665E21f);
	check(8388608.0f);
	check(1.6777216E7f);
	check(3.3554436E7f);
	check(6.7131496E7f);
	check(1.9310392E-38f);
	check(-2.47E-43f);
	check(1.993244E-38f);
	check(4103.9003f);
	check(5.3399997E9f);
	check(6.0898E-39f);
	check(0.0010310042f);
	check(2.8823261E17f);
	check(7.038531E-26f);
	check(9.2234038E17f);
	check(6.7108872E7f);
	check(1.0E-44f);
	check(2.816025E14f);
	check(9.223372E18f);
	check(1.5846085E29f);
	check(1.1811161E19f);
	check(5.368709E18f);
	check(4.6143165E18f);
	check(0.007812537f);
	check(1.4E-45f);
	check(1.18697724E20f);
	check(1.00014165E-36f);
	check(200.0f);
	check(3.3554432E7f);
	check(1.26217745E-29f);
}
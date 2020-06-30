
// a fast_io's experimental algorithm. I do not know whether it works.
namespace fast_io::details::cqwrteur
{
template<typename T,char8_t start>
inline constexpr auto calculate_table2()
{
	std::array<std::array<T,2>,100> gen{};
	for(char8_t i(0);i!=10;++i)
		for(char8_t j(0);j!=10;++j)
		{
			auto& ele(gen[static_cast<std::size_t>(i)*10+
			static_cast<std::size_t>(j)]);
			ele.front()=i+start;
			ele[1]=j+start;
		}
	return gen;
}

template<std::integral ch_type = char8_t,char8_t start=u8'0'>
class static_tables
{
public:
	using type = std::conditional_t<sizeof(ch_type)==1,char8_t,
		std::conditional_t<sizeof(ch_type)==2,char16_t,
		std::conditional_t<sizeof(ch_type)==4,char32_t,ch_type>>>;
	inline static constexpr auto table2{calculate_table2<type,start>()};
};

template<std::contiguous_iterator Iter>
inline void output_unsigned_9d(Iter str,std::uint32_t value)
{
	using ch_type = std::remove_cvref_t<decltype(*str)>;
	constexpr std::size_t bytes2{2*sizeof(ch_type)};
	//let's assume value = 9876543210
	std::uint32_t v3((value *2882303762ULL) >> 58U);//value/100000000=98
	std::uint32_t remain3(value-v3*100000000U);//value%100000000=76543210

	std::uint32_t v0((static_cast<std::uint64_t>(remain3)*static_cast<std::uint64_t>(3518437209)) >> 45U);//(value%100000000)/10000=987654
	std::uint32_t remain0(remain3-v0*10000U);//(value%100000000)%10000=value%10000=3210
	std::uint32_t v2((remain0*5243U)>>19U);//((value%100000000)%10000)/100=(value%10000)/100=32
	std::uint32_t remain2(remain0-v2*100U);//((value%100000000)%10000)%100=(value%10000)%100=10

	std::uint32_t v1((v0*3518437209ULL)>>45U);//((value%100000000)/10000)/10000=98
	std::uint32_t remain1(v0-v1*10000);//((value%100000000)/10000)%10000 = 7654


	std::uint32_t v5((remain1*5243U)>>19U);//((value%100000000)/10000)/100 = 76
	std::uint32_t remain5(remain1-v5*100U);//((value%100000000)/10000)%100 = 54

	*str = static_cast<char8_t>(v3)+u8'0';
	memcpy(str+1,static_tables<ch_type>::table2[v5].data(),bytes2);
	memcpy(str+3,static_tables<ch_type>::table2[remain5].data(),bytes2);
	memcpy(str+5,static_tables<ch_type>::table2[v2].data(),bytes2);
	memcpy(str+7,static_tables<ch_type>::table2[remain2].data(),bytes2);
}

template<std::contiguous_iterator Iter,my_unsigned_integral U>
requires (sizeof(U)<=16)
inline std::size_t output_unsigned(Iter str,U value)
{
	using ch_type = std::remove_cvref_t<decltype(*str)>;
	constexpr std::size_t bytes2{2*sizeof(ch_type)};
	if constexpr(sizeof(U)==8)
	{
		if (value >= 1000000000000000000ULL)
		{
			std::uint64_t v1(value/1000000000000000000ULL);
			std::uint64_t remain(value%1000000000000000000ULL);
			std::uint32_t v2(remain/1000000000ULL);
			std::uint32_t v3(remain%1000000000ULL);
			//8446744073709551615
			//8446744073709551615/1000000000
			if(v1>=10)
			{
				memcpy(str,static_tables<ch_type>::table2[v1].data(),bytes2);
				output_unsigned_9d(str+2,v2);
				output_unsigned_9d(str+11,v3);
				return 20;
			}
			else
			{
				*str=static_cast<char8_t>(v1)+u8'0';
				output_unsigned_9d(str+1,v2);
				output_unsigned_9d(str+10,v3);
				return 19;
			}
		}
		else if (value >= 1000000000ULL)
		{
			std::uint32_t v2(value/1000000000ULL);
			std::uint32_t v3(value%1000000000ULL);
			auto sz{output_unsigned(str,v2)};
			output_unsigned_9d(str+sz,v3);
			return sz+9;
		}
		else if (value >= 100000000)
		{
			//let's assume value = 9876543210
			std::uint32_t v3((value *2882303762ULL) >> 58U);//value/100000000=98
			std::uint32_t remain3(value-v3*100000000U);//value%100000000=76543210

			std::uint32_t v0((static_cast<std::uint64_t>(remain3)*static_cast<std::uint64_t>(3518437209)) >> 45U);//(value%100000000)/10000=987654
			std::uint32_t remain0(remain3-v0*10000U);//(value%100000000)%10000=value%10000=3210
			std::uint32_t v2((remain0*5243U)>>19U);//((value%100000000)%10000)/100=(value%10000)/100=32
			std::uint32_t remain2(remain0-v2*100U);//((value%100000000)%10000)%100=(value%10000)%100=10

			std::uint32_t v1((v0*3518437209ULL)>>45U);//((value%100000000)/10000)/10000=98
			std::uint32_t remain1(v0-v1*10000);//((value%100000000)/10000)%10000 = 7654


			std::uint32_t v5((remain1*5243U)>>19U);//((value%100000000)/10000)/100 = 76
			std::uint32_t remain5(remain1-v5*100U);//((value%100000000)/10000)%100 = 54

			if(value >= 1000000000)
			{
				memcpy(str,static_tables<ch_type>::table2[v3].data(),bytes2);
				memcpy(str+2,static_tables<ch_type>::table2[v5].data(),bytes2);
				memcpy(str+4,static_tables<ch_type>::table2[remain5].data(),bytes2);
				memcpy(str+6,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+8,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 10;
			}
			else
			{
				*str = static_cast<char8_t>(v3)+u8'0';
				memcpy(str+1,static_tables<ch_type>::table2[v5].data(),bytes2);
				memcpy(str+3,static_tables<ch_type>::table2[remain5].data(),bytes2);
				memcpy(str+5,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+7,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 9;
			}
		}
		else if (value >= 10000)
		{
			std::uint32_t v0((value *static_cast<std::uint64_t>(3518437209)) >> 45);
			std::uint32_t remain0(value-v0*10000U);
			std::uint32_t v2((remain0*5243U)>>19U);
			std::uint32_t remain2(remain0-v2*100U);

			if(v0>=100)
			{
				std::uint32_t v1((v0*5243U)>>19U);
				std::uint32_t remain1(v0-v1*100U);
				if(v0>=1000)
				{
					memcpy(str,static_tables<ch_type>::table2[v1].data(),bytes2);
					memcpy(str+2,static_tables<ch_type>::table2[remain1].data(),bytes2);
					memcpy(str+4,static_tables<ch_type>::table2[v2].data(),bytes2);
					memcpy(str+6,static_tables<ch_type>::table2[remain2].data(),bytes2);
					return 8;
				}
				else
				{
					*str = static_cast<char8_t>(v1)+u8'0';
					memcpy(str+1,static_tables<ch_type>::table2[remain1].data(),bytes2);
					memcpy(str+3,static_tables<ch_type>::table2[v2].data(),bytes2);
					memcpy(str+5,static_tables<ch_type>::table2[remain2].data(),bytes2);
					return 7;
				}
			}
			else if(v0>=10)
			{
				memcpy(str,static_tables<ch_type>::table2[v0].data(),bytes2);
				memcpy(str+2,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+4,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 6;
			}
			else
			{
				*str = static_cast<char8_t>(v0)+u8'0';
				memcpy(str+1,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+3,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 5;
			}
		}
		else if (value >= 100)
		{
			std::uint32_t v0((value*5243U)>>19U);
			std::uint32_t remain0(value-v0*100U);
			if(v0>=10)
			{
				memcpy(str,static_tables<ch_type>::table2[v0].data(),bytes2);
				memcpy(str+2,static_tables<ch_type>::table2[remain0].data(),bytes2);
				return 4;
			}
			else
			{
				*str = static_cast<char8_t>(v0)+u8'0';
				memcpy(str+1,static_tables<ch_type>::table2[remain0].data(),bytes2);
				return 3;
			}			
		}
		else if (value >= 10)
		{
			memcpy(str,static_tables<ch_type>::table2[value].data(),bytes2);
			return 2;
		}
		else
		{
			*str = static_cast<char8_t>(value)+u8'0';
			return 1;
		}
	}
	if constexpr(sizeof(U)==4)
	{
		if (value >= 100000000)
		{
			//let's assume value = 9876543210
			std::uint32_t v3((value *2882303762ULL) >> 58U);//value/100000000=98
			std::uint32_t remain3(value-v3*100000000U);//value%100000000=76543210

			std::uint32_t v0((static_cast<std::uint64_t>(remain3)*static_cast<std::uint64_t>(3518437209)) >> 45U);//(value%100000000)/10000=987654
			std::uint32_t remain0(remain3-v0*10000U);//(value%100000000)%10000=value%10000=3210
			std::uint32_t v2((remain0*5243U)>>19U);//((value%100000000)%10000)/100=(value%10000)/100=32
			std::uint32_t remain2(remain0-v2*100U);//((value%100000000)%10000)%100=(value%10000)%100=10

			std::uint32_t v1((v0*3518437209ULL)>>45U);//((value%100000000)/10000)/10000=98
			std::uint32_t remain1(v0-v1*10000);//((value%100000000)/10000)%10000 = 7654


			std::uint32_t v5((remain1*5243U)>>19U);//((value%100000000)/10000)/100 = 76
			std::uint32_t remain5(remain1-v5*100U);//((value%100000000)/10000)%100 = 54

			if(value >= 1000000000)
			{
				memcpy(str,static_tables<ch_type>::table2[v3].data(),bytes2);
				memcpy(str+2,static_tables<ch_type>::table2[v5].data(),bytes2);
				memcpy(str+4,static_tables<ch_type>::table2[remain5].data(),bytes2);
				memcpy(str+6,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+8,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 10;
			}
			else
			{
				*str = static_cast<char8_t>(v3)+u8'0';
				memcpy(str+1,static_tables<ch_type>::table2[v5].data(),bytes2);
				memcpy(str+3,static_tables<ch_type>::table2[remain5].data(),bytes2);
				memcpy(str+5,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+7,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 9;
			}
		}
		else if (value >= 10000)
		{
			std::uint32_t v0((static_cast<std::uint64_t>(value) *static_cast<std::uint64_t>(3518437209)) >> 45);
			std::uint32_t remain0(value-v0*10000U);
			std::uint32_t v2((remain0*5243U)>>19U);
			std::uint32_t remain2(remain0-v2*100U);

			if(v0>=100)
			{
				std::uint32_t v1((v0*5243U)>>19U);
				std::uint32_t remain1(v0-v1*100U);
				if(v0>=1000)
				{
					memcpy(str,static_tables<ch_type>::table2[v1].data(),bytes2);
					memcpy(str+2,static_tables<ch_type>::table2[remain1].data(),bytes2);
					memcpy(str+4,static_tables<ch_type>::table2[v2].data(),bytes2);
					memcpy(str+6,static_tables<ch_type>::table2[remain2].data(),bytes2);
					return 8;
				}
				else
				{
					*str = static_cast<char8_t>(v1)+u8'0';
					memcpy(str+1,static_tables<ch_type>::table2[remain1].data(),bytes2);
					memcpy(str+3,static_tables<ch_type>::table2[v2].data(),bytes2);
					memcpy(str+5,static_tables<ch_type>::table2[remain2].data(),bytes2);
					return 7;
				}
			}
			else if(v0>=10)
			{
				memcpy(str,static_tables<ch_type>::table2[v0].data(),bytes2);
				memcpy(str+2,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+4,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 6;
			}
			else
			{
				*str = static_cast<char8_t>(v0)+u8'0';
				memcpy(str+1,static_tables<ch_type>::table2[v2].data(),bytes2);
				memcpy(str+3,static_tables<ch_type>::table2[remain2].data(),bytes2);
				return 5;
			}
		}
		else if (value >= 100)
		{
			std::uint32_t v0((value*5243U)>>19U);
			std::uint32_t remain0(value-v0*100U);
			if(v0>=10)
			{
				memcpy(str,static_tables<ch_type>::table2[v0].data(),bytes2);
				memcpy(str+2,static_tables<ch_type>::table2[remain0].data(),bytes2);
				return 4;
			}
			else
			{
				*str = static_cast<char8_t>(v0)+u8'0';
				memcpy(str+1,static_tables<ch_type>::table2[remain0].data(),bytes2);
				return 3;
			}			
		}
		else if (value >= 10)
		{
			memcpy(str,static_tables<ch_type>::table2[value].data(),bytes2);
			return 2;
		}
		else
		{
			*str = static_cast<char8_t>(value)+u8'0';
			return 1;
		}
	}
	else if constexpr(sizeof(U)==2)
	{
		if (value >= 10000)
		{
			std::uint32_t v0((value*107375ULL)>>30U);
			std::uint32_t remain0(value-v0*10000U);
			std::uint32_t v1((remain0*5243U)>>19U);
			std::uint32_t remain1(remain0-v1*100U);
			*str = static_cast<char8_t>(v0)+u8'0';
			memcpy(str+1,static_tables<ch_type>::table2[v1].data(),bytes2);
			memcpy(str+3,static_tables<ch_type>::table2[remain1].data(),bytes2);
			return 5;
		}
		else if (value >= 100)
		{
			std::uint32_t v0((value*5243U)>>19U);
			std::uint32_t remain0(value-v0*100U);
			if(v0>=10)
			{
				memcpy(str,static_tables<ch_type>::table2[v0].data(),bytes2);
				memcpy(str+2,static_tables<ch_type>::table2[remain0].data(),bytes2);
				return 4;
			}
			else
			{
				*str = static_cast<char8_t>(v0)+u8'0';
				memcpy(str+1,static_tables<ch_type>::table2[remain0].data(),bytes2);
				return 3;
			}			
		}
		else if (value >= 10)
		{
			memcpy(str,static_tables<ch_type>::table2[value].data(),bytes2);
			return 2;
		}
		else
		{
			*str = static_cast<char8_t>(value)+u8'0';
			return 1;
		}
	}
	else if constexpr(sizeof(U)==1)
	{
		if (value >= 100)
		{
			char8_t v0((value*41)>>12);
			char8_t remain0(value-v0*100);
			*str = static_cast<char8_t>(v0)+u8'0';
			memcpy(str+1,static_tables<ch_type>::table2[remain0].data(),bytes2);
			return 3;
		}
		else if (value >= 10)
		{
			memcpy(str,static_tables<ch_type>::table2[value].data(),bytes2);
			return 2;
		}
		else
		{
			*str = static_cast<char8_t>(value)+u8'0';
			return 1;
		}
	}
}

}
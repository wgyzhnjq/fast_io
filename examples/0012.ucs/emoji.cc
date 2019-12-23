#include"../../include/fast_io.h"
#include"../../include/fast_io_device.h"

int main()
{
	using namespace fast_io;
	
	fast_io::ucs<fast_io::obuf,char32_t> view("emoji.txt");
	auto &ob(view.native_handle());

	std::string_view const emoji("☭🚄😀我wubi😄😊😉😍😘😚😜😝😳😁😣😢😂😭😪😥😰😩㍿🀀🀁🀂🀃🀄🀅🀆🀇🀈🀉🀊🀋🀌🀍🀎🀏");
	println(ob,u8"original emoji string:\n",emoji);
	for(auto const & e : emoji)
		print(ob,unsigned_view(e),u8" ");
	print(ob,u8"\n\n");
	auto u32emoji(utf8_to_ucs<std::u32string>(emoji));
	println(ob,u8"u32string size is ",u32emoji.size(),u8"\nucs string:");

	print(view,u32emoji);
	print(ob,u8"\n\n");
	for(auto const & e : u32emoji)
	{
		put(view,e);
		fprint(ob,u8" % (0x%)\n",unsigned_view(e),hexupper(unsigned_view(e)));
	}
	put(ob,u8'\n');
	auto const convert_back(ucs_to_utf8(u32emoji));
	println(ob,u8"convert back emoji string:\n",emoji);
	for(auto const& e : convert_back)
		print(ob,unsigned_view(e),u8" ");
	println(ob,u8"\n\nSame?\n",emoji==convert_back);
}
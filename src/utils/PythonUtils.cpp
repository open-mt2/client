#include <Client/Utils/PythonUtils.h>
#include <Platform/Platform.h>
#include <string>

namespace Utils::Python
{
	const char* const pythonLibraries[] =
		{
			"abc.pyc",
			"bisect.pyc",
			"codecs.pyc",
			"collections.pyc",
			"copy.pyc",
			"copy_reg.pyc",
			"fnmatch.pyc",
			"functools.pyc",
			"genericpath.pyc",
			"heapq.pyc",
			"hashlib.pyc",
			"keyword.pyc",
			"linecache.pyc",
			"locale.pyc",
			"ntpath.pyc",
			"os.pyc",
			"random.pyc",
			"re.pyc",
			"shutil.pyc",
			"site.pyc",
			"sre_compile.pyc",
			"sre_constants.pyc",
			"sre_parse.pyc",
			"stat.pyc",
			"string.pyc",
			"sysconfig.pyc",
			"traceback.pyc",
			"types.pyc",
			"UserDict.pyc",
			"warnings.pyc",
			"weakref.pyc",
			"_abcoll.pyc",
			"_weakrefset.pyc",
			"__future__.pyc",
			"encodings/aliases.pyc",
			"encodings/ascii.pyc",
			"encodings/cp1250.pyc",
			"encodings/cp1251.pyc",
			"encodings/cp1252.pyc",
			"encodings/cp949.pyc",
			"encodings/utf_8.pyc",
			"encodings/cp850.pyc",
			"encodings/__init__.pyc",
			"\n",
	};

	bool CheckPythonLibraryFilenames()
	{
		for (int i = 0; *pythonLibraries[i] != '\n'; ++i)
		{
			std::string filename = "lib/";
			filename += pythonLibraries[i];

			for (char& character : filename)
			{
				if (character == '\\')
					character = '/';
			}

			if (_access(filename.c_str(), 0) != 0)
				return false;

			MoveFile(filename.c_str(), filename.c_str());
		}

		return true;
	}
}

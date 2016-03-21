#include "stdafx.h"
#include <fstream>
#include <Common/FileIO.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(FileIOTests)
	{
		static std::string getTestFilePath()
		{
			return "D:\\fileIoTestFIle.txt";
		}

		std::ifstream stream;
	public:

		TEST_CLASS_INITIALIZE(createTestFile)
		{
			std::ofstream stream(getTestFilePath(), std::fstream::out | std::fstream::ate);

			stream << "$ FILTRE" << std::endl;
			stream << "OFF" << std::endl;
			stream << "$ PREDYKCJA" << std::endl;
			stream << "$ WIDTH" << std::endl;
			stream << "4" << std::endl;
			stream << "$ HEIGHT" << std::endl;
			stream << "4" << std::endl;
			stream << "$ MATRIX" << std::endl;
			stream << "62 73 70 56" << std::endl;
			stream << "63 49 49 61" << std::endl;
			stream << "55 67 73 73" << std::endl;
			stream << "73 73 73 73" << std::endl;

			stream.close();
		}

		TEST_CLASS_CLEANUP(deleteTestFile)
		{
			remove(getTestFilePath().c_str());
		}

		TEST_METHOD_INITIALIZE(openStreamToTestFile)
		{
			stream.open(getTestFilePath());
		}

		TEST_METHOD_CLEANUP(closeStreamToTestFile)
		{
			stream.close();
		}

		TEST_METHOD(goToMarker_markerExists_returnsTrue)
		{
			auto result = HEVC::FileIO::goToMarker(stream, "FILTRE");

			Assert::IsTrue(result);
		}

		TEST_METHOD(goToMarker_markerDoesntExists_returnsFalse)
		{
			auto result = HEVC::FileIO::goToMarker(stream, "NoSuchMarker");

			Assert::IsFalse(result);
		}

		TEST_METHOD(readValue_valueExists_returnsTrue)
		{
			size_t width;

			auto result = HEVC::FileIO::readValue(stream, "WIDTH", width);

			Assert::IsTrue(result);
			Assert::AreEqual(width, 4u);
		}

		TEST_METHOD(readMatrix_valueExists_returnsTrue)
		{
			auto result = HEVC::FileIO::readMatrix<int>(stream, "PREDYKCJA");

			Assert::IsTrue(result != nullptr);

			Assert::AreEqual(result->at(0, 0), 62);
			Assert::AreEqual(result->at(1, 1), 49);
			Assert::AreEqual(result->at(2, 2), 73);
			Assert::AreEqual(result->at(3, 3), 73);

			Assert::AreEqual(result->at(0, 3), 73);
			Assert::AreEqual(result->at(3, 0), 56);
		}
	};
}
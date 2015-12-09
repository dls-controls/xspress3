#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Fixtures
#include <boost/test/unit_test.hpp>

#include <stdio.h>
#include "xspress3.h"
#include "xspress3EpicsTest.h"

#define MAX_SPECTRA 4096
#define NUM_CHANNELS 10

// asynPorts seem to hang around even after the xspress destructor so use a new port name for each test suite.
char asynPortHack = 1;

Xspress3Det::Xspress3Det()
{
    xsp = new Xspress3(&asynPortHack, NUM_CHANNELS);
}

void Xspress3Det::connect()
{
    this->xsp->connect();
}

bool Xspress3Det::createMCAArray(NDDataType_t dataType)
{
    size_t dims[2] = {MAX_SPECTRA, NUM_CHANNELS};
    NDArray *pMCA;
    return this->xsp->createMCAArray(dims, pMCA, dataType);
}

bool Xspress3Det::readFrameUInt()
{
    u_int32_t SCA[XSP3_SW_NUM_SCALERS], MCAData[MAX_SPECTRA * NUM_CHANNELS];
    return this->xsp->readFrame(&SCA[0], &MCAData[0], 0, MAX_SPECTRA);
}

bool Xspress3Det::readFrameDouble()
{
    double *SCA, *MCAData;
    bool result;
    SCA = new double[XSP3_SW_NUM_SCALERS];
    MCAData = new double[MAX_SPECTRA * NUM_CHANNELS];
    result = this->xsp->readFrame(SCA, MCAData, 0, MAX_SPECTRA);
    // It looks like hist_dtc_read4d does something funny with SCA as
    // delete causes a SIGABRT when freeing so lets just leak it for
    // now.
    delete[] MCAData;
    return result;
}

struct xspress3Fixture
{
    Xspress3Det xspDet;
    Xspress3Det *pXspDet;
 
    xspress3Fixture()
    {
        pXspDet = &xspDet;
        xspDet.connect();
        asynPortHack++;
    }
 
    ~xspress3Fixture()
    {
        if (pXspDet == NULL)
            BOOST_TEST_MESSAGE("xsp not null");
    }
};

BOOST_FIXTURE_TEST_SUITE(support, xspress3Fixture)

BOOST_AUTO_TEST_CASE(createMCAArrayUInt32)
{
    BOOST_CHECK(xspDet.createMCAArray(NDUInt32) == true);
}

BOOST_AUTO_TEST_CASE(createMCAArrayFloat64)
{
    BOOST_CHECK(xspDet.createMCAArray(NDFloat64) == true);
}

BOOST_AUTO_TEST_CASE(readFrameDouble)
{
    BOOST_CHECK(xspDet.readFrameDouble() == false);
}

BOOST_AUTO_TEST_CASE(readFrameUInt)
{
    BOOST_CHECK(xspDet.readFrameUInt() == false);
}

BOOST_AUTO_TEST_SUITE_END()
    
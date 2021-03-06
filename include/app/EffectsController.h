/*************************************************
 * @file EffectsController.h
 * @brief Effects controller interface
 *
 *************************************************/
#pragma once

#include <list>
#include <string>
#include <utility>
#include "stdint.h"

#include "app/Color.h"
#include "app/RGBFixture.h"
#include "app/TaskState.h"
#include "drv/DRVGPIO.h"
#include "queue.hpp"
#include "thread.hpp"

class EffectBase
{
public:
    virtual ~EffectBase();

    virtual void reset(uint32_t tick) = 0;

    virtual void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset) = 0;
};

/**
 * EffectsController
 */
class EffectsController : public cpp_freertos::Thread, TaskState
{
public:
    enum EffectMode
    {
        EffectModeDmx,
        EffectModeRainbow,
        EffectModeTest
    };

    typedef std::pair<uint16_t, uint16_t>    DmxRange;
    typedef std::pair<EffectBase&, DmxRange> DmxEffect;

    EffectsController(const char* Name, TaskHandle_t taskToNotify, uint8_t ID, cpp_freertos::Queue& inputQueue, cpp_freertos::Queue& outputQueue,
                      uint32_t interval);

    EffectsController(const char* Name, TaskHandle_t taskToNotify, uint8_t ID, cpp_freertos::Queue& inputQueue, cpp_freertos::Queue& outputQueue,
                      uint32_t interval, GPIOpin* testMode1, GPIOpin* testMode2);

    void addFixture(RGBFixture fixture);

    void addEffect(EffectBase& effect, DmxRange range);

    void Run();

    void       setMode(EffectMode mode);
    EffectMode getMode();

protected:
private:
    enum DMXChannelConfig
    {
        DMX_CHN_RED = 0,
        DMX_CHN_GREEN,
        DMX_CHN_BLUE,
        DMX_CHN_MODE,
    };

    std::list<RGBFixture> m_fixtures;
    std::list<DmxEffect>  m_effects;
    DmxEffect*            m_lastEffect;
    cpp_freertos::Queue   m_inputQueue;
    cpp_freertos::Queue   m_outputQueue;
    uint32_t              m_interval;
    uint8_t               m_dmxSelector;
    Color                 m_suggestedColor;
    GPIOpin*              m_testModePin1;
    GPIOpin*              m_testModePin2;
    uint32_t              m_testEffectCounter;
    EffectMode            m_effectMode;

    EffectsController();  // not allowed

    DmxEffect* getApplicableEffect(uint8_t dmxSelector);

    bool isEffectOverlapping();

    static bool listCompareDmxRange(const DmxEffect a, const DmxEffect b);

    static bool listCompareFixtureLocation(const RGBFixture a, const RGBFixture b);

    void applyTestEffect(bool selectEffect);
};

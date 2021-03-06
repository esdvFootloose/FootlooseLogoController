/*************************************************
 * @file UserInterface.cpp
 * @brief User interface
 *
 *************************************************/
#include "app/UserInterface.h"

#include "app/UserInterfaceFSM.h"
#include "os/thread.hpp"

using namespace std;
using namespace statemap;

UserInterface::UserInterface(EffectsController& effectsController, DRV7SegmentDisplay& display, GPIOpin& power, GPIOpin& status, BinDecIO& dmxAddress,
                             GPIOpin& btnOk, GPIOpin& btnMode, GPIOpin& swMode, CAT5932& driver1, CAT5932& driver2)
    : cpp_freertos::Thread("UI", 250, 2)
    , m_uiLedPower(power)
    , m_uiLedStatus(status)
    , m_dmxAddress(dmxAddress)
    , m_fsm(*this)
    , m_effectsController(effectsController)
    , m_display(display)
    , m_btnOk(btnOk)
    , m_btnMode(btnMode)
    , m_swMode(swMode)
    , m_lastBtnStateOk(false)
    , m_lastBtnStateMode(false)
    , m_displayBrightness(10)
    , m_ledDriver1(driver1)
    , m_ledDriver2(driver2)
    , m_lastDmxAddress(0)
    , m_previousMode(OperationDmx)
{
    // TODO: load and save mode and display brightness from eeprom
    m_updateTimer.subscribe(&updateTimerElapsed, this);
    m_displaySleepTimer.subscribe(&displaySleepTimerElapsed, this);
}

void UserInterface::Run()
{
    m_fsm.Ready();

    while (1)
    {
        if ((m_btnOk && !m_lastBtnStateOk) && !m_btnMode)
        {
            m_fsm.BtnOkPressed();
        }
        if ((m_btnMode && !m_lastBtnStateMode) && !m_btnOk)
        {
            m_fsm.BtnModePressed();
        }
        m_lastBtnStateMode = m_btnMode;
        m_lastBtnStateOk   = m_btnOk;

        if (m_swMode)
        {
            m_fsm.SwModeEnabled();
        }
        else
        {
            m_fsm.SwModeDisabled();
        }

        if (m_dmxAddress != m_lastDmxAddress)
        {
            m_fsm.DmxAddressChanged();
        }
        m_lastDmxAddress = m_dmxAddress;

        m_fsm.Tick();
        Delay(100);
    }
}

void UserInterface::updateTimerElapsed(bool running, void* ptr)
{
    (void)running;
    UserInterface* This = static_cast<UserInterface*>(ptr);
    This->m_fsm.updateTimerElapsed();
}

void UserInterface::displaySleepTimerElapsed(bool running, void* ptr)
{
    (void)running;
    UserInterface* This = static_cast<UserInterface*>(ptr);
    This->m_fsm.displaySleepTimerElapsed();
}

void UserInterface::uiLedPower(bool state) { m_uiLedPower = state; }

void UserInterface::uiLedStatus(bool state) { m_uiLedStatus = state; }

void UserInterface::uiDisplay(uint16_t value) { m_display.setNumber(value); }

void UserInterface::uiDisplay(const char* str) { m_display.setString(str); }

void UserInterface::setMode(OperationModes mode)
{
    m_previousMode = static_cast<OperationModes>(m_effectsController.getMode());
    m_effectsController.setMode(static_cast<EffectsController::EffectMode>(mode));
}
void UserInterface::restorePreviousMode() { m_effectsController.setMode(static_cast<EffectsController::EffectMode>(m_previousMode)); }

uint8_t UserInterface::getDisplayBrightness() { return m_displayBrightness; }
void    UserInterface::incDisplayBrightness()
{
    m_displayBrightness++;
    if (m_displayBrightness > 10) m_displayBrightness = 0;
    m_ledDriver1.setBrightness((m_displayBrightness * 25) + 5);
    m_ledDriver2.setBrightness((m_displayBrightness * 25) + 5);
}

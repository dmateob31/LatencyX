#include "system_timer.hpp"
#include <timeapi.h>

SystemTimer::SystemTimer() 
    : m_currentResolution(0), m_isActive(false) {}

SystemTimer::~SystemTimer() {
    if (m_isActive) {
        RestoreResolution();
    }
}

bool SystemTimer::SetResolution(UINT milliseconds) {
    if (milliseconds == 0) return false;

    // Si ya habia una resolucion activa, se libera primero.
    if (m_isActive) {
        RestoreResolution();
    }

    MMRESULT result = timeBeginPeriod(milliseconds);
    if (result == TIMERR_NOERROR) {
        m_currentResolution = milliseconds;
        m_isActive = true;
        return true;
    }

    return false;
}

bool SystemTimer::RestoreResolution() {
    if (!m_isActive) return true;

    MMRESULT result = timeEndPeriod(m_currentResolution);
    if (result == TIMERR_NOERROR) {
        m_isActive = false;
        m_currentResolution = 0;
        return true;
    }

    return false;
}

bool SystemTimer::IsActive() const {
    return m_isActive;
}

UINT SystemTimer::GetCurrentResolution() const {
    return m_currentResolution;
}
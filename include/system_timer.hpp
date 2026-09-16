#ifndef SYSTEM_TIMER_HPP
#define SYSTEM_TIMER_HPP

#include <windows.h>

class SystemTimer {
public:
    SystemTimer();
    ~SystemTimer();

    // Establece la resolucion deseada en milisegundos (por ejemplo, 1 para 1.0 ms).
    bool SetResolution(UINT milliseconds);

    // Restaura la resolucion predeterminada del sistema operativo.
    bool RestoreResolution();

    // Devuelve si el ajuste esta activo actualmente.
    bool IsActive() const;

    // Obtiene la resolucion activa configurada.
    UINT GetCurrentResolution() const;

private:
    UINT m_currentResolution;
    bool m_isActive;
};

#endif // SYSTEM_TIMER_HPP
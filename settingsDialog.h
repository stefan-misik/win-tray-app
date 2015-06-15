#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "common.h"

typedef struct _SETTINGSDATA
{
	INT a;
} SETTINGSDATA, *LPSETTINGSDATA;



INT_PTR SettingsShow(
	LPSETTINGSDATA lpData
	);

#endif /* SETTINGSDIALOG_H */
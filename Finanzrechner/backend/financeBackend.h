#pragma once
#include "stdafx.h"

#include "data/transaction.h"
#include "data/month.h"
#include "dataFunctions/m_container.h"
#include "dataFunctions/calc_evaluating.h"
#include "dataFunctions/filter.h"
#include "dataFunctions/dataObtain.h"
#include "settings/settings.h"
#include "fileHandler/fh.h"

#ifdef compileWithCrypt
#include "fileHandler/cryptFileHandler/cryptFH.h"
#else
#include "fileHandler/plainFileHandler/plainFH.h"
#endif
#pragma once

#ifdef SCARLENTT_DEBUG
#define SCARLENTT_ASSERT(x) assert(x)
#else // IF SCARLETT_DEBUG.
#define SCARLENTT_ASSERT(x)
#endif // IF NOT SCARLETT_DEBUG.

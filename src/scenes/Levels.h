#pragma once

namespace ltrm
{

// 25 Levels and Solutions (not necessarily optimal/only)

// I came up with most of these by coming up with a starting
// and ending word and solving. Then I ranked by difficulty.

static constexpr size_t numLevels = 15;
static constexpr const char* levels[] = {
  // EASY
  "CAT-BAR", // CAT -> CAR -> BAR
  "RATE-LAME", // RATE -> LATE -> LAME
  "PANT-CART", // PANT -> PART -> CART
  "SPAT-STAR", // STAR -> STAT -> SPAT
  "MAGIC-PANIC", // MAGIC -> MANIC -> PANIC
  
  // MEDIUM
  "MILE-HARE", // MILE -> MALE -> MARE -> HARE -> HARP
  "LOOP-CREW", // CREW -> CROW -> CROP -> COOP -> LOOP
  "SPACE-SHORT", // SPACE -> SPARE -> SPORE -> SHORE -> SHORT
  "FINE-RICE", // RICE -> RACE -> LACE -> LINE -> FINE
  "WILD-TANK", // TANK -> TALK -> TALL -> WALL -> WILL -> WILD
  
  // VERY HARD
  "PACE-DARE", // DARE -> DATE -> MATE -> MACE -> PACE
  "GRID-WAND", // WAND -> WANT -> WAIT -> WRIT -> GRIT -> GRID
  "PANE-PAIN", // PAIN -> PAID -> LAID -> LAND -> LANE -> PANE
  "PAIR-LEAD", // PAIR -> HAIR -> HEIR -> HEAR -> HEAD -> LEAD -> BEAR
  "STEEP-SPEAK", // SPEAK -> SPEAR -> SHEAR -> SHEER -> SHEEP -> STEEP
};

}

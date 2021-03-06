/* ASEPRITE
 * Copyright (C) 2001-2013  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef UNDOERS_FLIP_IMAGE_H_INCLUDED
#define UNDOERS_FLIP_IMAGE_H_INCLUDED

#include "gfx/fwd.h"
#include "raster/algorithm/flip_type.h"
#include "undo/object_id.h"
#include "undoers/undoer_base.h"

class Image;

namespace undoers {

class FlipImage : public UndoerBase
{
public:
  FlipImage(undo::ObjectsContainer* objects, Image* image,
            const gfx::Rect& bounds,
            raster::algorithm::FlipType flipType);

  void dispose() OVERRIDE;
  size_t getMemSize() const OVERRIDE { return sizeof(*this); }
  void revert(undo::ObjectsContainer* objects, undo::UndoersCollector* redoers) OVERRIDE;

private:
  undo::ObjectId m_imageId;
  uint8_t m_format;
  uint16_t m_x, m_y, m_w, m_h;
  uint8_t m_flipType;
};

} // namespace undoers

#endif  // UNDOERS_FLIP_IMAGE_H_INCLUDED

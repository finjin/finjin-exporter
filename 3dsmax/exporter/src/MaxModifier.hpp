//Copyright (c) 2017 Finjin
//
//This file is part of Finjin Exporter (finjin-exporter).
//
//Finjin Exporter is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//Finjin Exporter is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with Finjin Exporter.  If not, see <http://www.gnu.org/licenses/>.


#pragma once


//Classes----------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /** Wraps access to a 3DS Max modifier. */
    class MaxModifier
    {
    public:
        /** Default constructor. */
        MaxModifier();

        /** Constructor that takes a modifier. */
        MaxModifier(Modifier* modifier);

        /** Constructor that takes a modifier. */
        MaxModifier(const MaxModifier& maxModifier);

        /** Removes the modifier and sets the original state to false. */
        void Clear();

        /** Determines if the modifier is non-null. */
        bool IsValid() const;

        /** Overloaded -> operator that allows MaxModifier to be used like a pointer. */
        Modifier* operator -> ();

        /** Returns the Max modifier. */
        operator Modifier* ();

        /** Enables/disables the modifier. */
        void Enable(bool enable = true);

        /** Disables the modifier. */
        void Disable();

        /** Restores the modifier's enabled/disabled state. */
        void Restore();

    public:
        Modifier* modifier;
        BOOL originalState;
    };

} }

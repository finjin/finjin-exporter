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


//Types-------------------------------------------------------------------------
namespace Finjin { namespace Exporter {

    /**
     * A hierarchical collection of progress calculators. Having a hierarchy
     * allows a collection of small events to have their progress tracked separately
     * and aggregated as a larger event, such as loading a file and all of the
     * objects in that file. Progress is calculated as a number between 0 and 1, inclusive.
     */
    class ProgressCalculator
    {
    public:
        ProgressCalculator(ProgressCalculator* parent = nullptr);
        ProgressCalculator(const wxString& name, ProgressCalculator* parent = nullptr);
        virtual ~ProgressCalculator();

        /** Gets the name of the calculator. */
        const wxString& GetName() const;

        /** Calculates and returns the total progress, on a scale of 0 to 1. */
        float GetProgress();

        /**
         * Manually sets the progress.
         * This will not normally need to be called, but it is useful in circumstances
         * where there are multiple progress calculators virtually connected, and the
         * progress of one calculator needs to be input into a separate one elsewhere.
         * Calling this is pointless if the calculator has child calculators.
         * @param progress [in] - The total progress, on a scale of 0 to 1
         */
        void UpdateProgress(float progress);

        /**
         * Updates the progress for this calculator.
         * Calling this is pointless if the calculator has child calculators.
         * @param amount [in] - The amount of progress to add. The amount is
         * in the same units as those passed into SetRange(). The amount may
         * be positive or negative
         */
        void Update(float amount);

        /**
         * Gets the current value range.
         * Calling this is pointless if the calculator has child calculators.
         */
        float GetRange() const;

        /**
         * Sets the value range of this calculator.
         * Note that calling this method resets the progress to 0.
         * Also note that calling this is pointless if the calculator has child calculators
         * @param range [in] - The range is used to scale amounts passed to Update() and, in turn,
         * calculate the total progress. As an example, if using the calculator
         * to track the number of objects loaded, SetRange() should be called with
         * the total number of objects, and Update(1) should be called every
         * time one object is loaded.
         */
        void SetRange(float range);

        /**
         * Creates a child calculator.
         * @param name [in] - The name of the calculator.
         * @return A child calculator is returned. This calculator should not be deleted.
         */
        std::shared_ptr<ProgressCalculator> AddCalculator(const wxString& name);

        void RemoveCalculators();

        class ProgressChangeListener
        {
        public:
            virtual ~ProgressChangeListener() {}

            virtual void ProgressChanged(ProgressCalculator* progress) = 0;
        };

        void AddProgressChangeListener(ProgressChangeListener* listener);
        void RemoveProgressChangeListener(ProgressChangeListener* listener);

    protected:
        void ProgressChanged(ProgressCalculator* source = nullptr);

    protected:
        ProgressCalculator* root;
        ProgressCalculator* parent;

        /** The name of the calculator. This is provided for the user's benefit. */
        wxString name;

        /** The current progress. */
        float progress;

        /** The range used to scale progress to [0,1]. */
        float range;

        /** Child calculators. */
        std::list<std::shared_ptr<ProgressCalculator> > childCalculators;

        std::list<ProgressChangeListener*> progressChangeListeners;
    };

} }

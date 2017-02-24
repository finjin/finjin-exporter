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


//Includes----------------------------------------------------------------------
#include "FinjinPrecompiled.hpp"
#include "ProgressCalculator.hpp"

using namespace Finjin::Exporter;


//Implementation---------------------------------------------------------------
ProgressCalculator::ProgressCalculator(ProgressCalculator* parent)
{
    this->root = (parent != nullptr) ? parent->root : this;
    this->parent = parent;    
    this->progress = 0;
    this->range = 0;
}

ProgressCalculator::ProgressCalculator(const wxString& name, ProgressCalculator* parent)
{
    this->root = (parent != nullptr) ? parent->root : this;
    this->parent = parent;    
    this->name = name;
    this->progress = 0;
    this->range = 0;
}

ProgressCalculator::~ProgressCalculator()
{
    RemoveCalculators();
}

const wxString& ProgressCalculator::GetName() const
{
    return this->name;
}

float ProgressCalculator::GetProgress()
{
    if (!this->childCalculators.empty())        
    {
        //Reset progress
        this->progress = 0;

        //Determine the influence of each child calculator
        float influence = (float)1.0/this->childCalculators.size();        

        //Add the progress of all child calculators
        for (auto calculator : this->childCalculators)
            this->progress += calculator->GetProgress() * influence;
    }
    
    return this->progress;
}

void ProgressCalculator::UpdateProgress(float progress)
{
    if (progress != this->progress)
    {
        this->progress = progress;
        ProgressChanged(this);
    }
}

void ProgressCalculator::Update(float amount)
{
    if (this->range > 0)
    {
        //Update the progress, scaling it by the inverse range
        this->progress += amount / this->range;

        //Limit the progress to [0, 1]    
        this->progress = std::max(this->progress, (float)0);
        this->progress = std::min(this->progress, (float)1);    

        ProgressChanged(this);
    }
}

float ProgressCalculator::GetRange() const
{
    return this->range;
}

void ProgressCalculator::SetRange(float range)
{
    this->range = range;
    this->progress = 0;
}

std::shared_ptr<ProgressCalculator> ProgressCalculator::AddCalculator(const wxString& name)
{
    std::shared_ptr<ProgressCalculator> calculator(new ProgressCalculator(name, this));
    this->childCalculators.push_back(calculator);
    return calculator;
}

void ProgressCalculator::RemoveCalculators()
{
    this->childCalculators.clear();
}

void ProgressCalculator::AddProgressChangeListener(ProgressChangeListener* listener)
{
    if (std::find(this->progressChangeListeners.begin(), this->progressChangeListeners.end(), listener) == this->progressChangeListeners.end())
        this->progressChangeListeners.push_back(listener);
}

void ProgressCalculator::RemoveProgressChangeListener(ProgressChangeListener* listener)
{
    this->progressChangeListeners.remove(listener);
}

void ProgressCalculator::ProgressChanged(ProgressCalculator* source)
{
    if (source == nullptr)
        source = this;

    for (auto calculator : this->progressChangeListeners)
    {
        calculator->ProgressChanged(source);
    }

    if (this->parent != nullptr)
        this->parent->ProgressChanged(source);
}

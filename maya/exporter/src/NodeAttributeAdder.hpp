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
     * Utility class used to create node attributes.
     *
     * Note that nearly all of the 'array' methods set a default value that is
     * unlikely to be used in normal program operation. This is due to the fact that
     * Maya does not save array elements that contain the default values, which could cause
     * problems if you have an array and depend on it having a the number of elements as it
     * had when the file was originally created.
     *
     * By default, attributes are added as inputs. Call MakeOutput() to change this behavior.
     */
    class NodeAttributeAdder
    {
    public:
        NodeAttributeAdder();

        /** Modifies the adder so that all added attributes are inputs. */
        void MakeInput(bool keyable = false);

        /** Modifies the adder so that all added attributes are outputs. */
        void MakeOutput();

        /**
         * Adds a an integer attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddInt(const MString& name, int defaultValue = 0);

        /**
         * Adds a an integer attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @param minValue [in] - The minimum value.
         * @param maxValue [in] - The maximum value.
         * @return The created attribute is returned.
         */
        MObject AddInt(const MString& name, int defaultValue, int minValue, int maxValue);

        /**
         * Adds a an integer array attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddIntArray(const MString& name);

        /**
         * Adds a enum attribute that has no values.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddEmptyEnum(const MString& name);

        /**
         * Adds a enum attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @param pairCount [in] - The number of variable argument pairs, each of which represents
         * an enum name (a string) and its value (a short).
         * @return The created attribute is returned.
         */
        MObject AddEnum(const MString& name, short defaultValue = 0, int pairCount = 0, ...);

        /**
         * Adds a enum (with no value) array attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddEmptyEnumArray(const MString& name);

        /**
         * Adds a enum array attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @param pairCount [in] - The number of variable argument pairs, each of which represents
         * an enum name (a string) and its value (a short).
         * @return The created attribute is returned.
         */
        MObject AddEnumArray(const MString& name, short defaultValue = 0, int pairCount = 0, ...);

        /**
         * Adds a distance attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddDistance(const MString& name, MDistance defaultValue);

        /**
         * Adds a distance attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddDistance(const MString& name, MDistance defaultValue, double maxValue);

        /**
         * Adds a float attribute to be used as the alpha component of a color.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddAlpha(const MString& name, double defaultValue = 1);

        /**
         * Adds a double attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddDouble(const MString& name, double defaultValue = 0);

        /**
         * Adds a float attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @param minValue [in] - The minimum value.
         * @param maxValue [in] - The maximum value.
         * @return The created attribute is returned.
         */
        MObject AddFloat(const MString& name, double defaultValue = 0, double minValue = 0, double maxValue = 100);

        /**
         * Adds a float array attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddFloatArray(const MString& name);

        /**
         * Adds a boolean attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddBool(const MString& name, bool defaultValue = false);

        /**
         * Adds a color attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddColor(const MString& name, MColor defaultValue = MColor(0,0,0));

        /**
         * Adds a 2-float attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddFloat2(const MString& name, MPoint defaultValue = MPoint(0,0,0,0));

        /**
         * Adds a 3-float attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddFloat3(const MString& name, MPoint defaultValue = MPoint(0,0,0,0));

        /**
         * Adds a 3-float array attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddFloat3Array(const MString& name);

        /**
         * Adds a 4-double attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddDouble4(const MString& name, MPoint defaultValue = MPoint(0,0,0,0));

        /**
         * Adds a 4-double array attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddDouble4Array(const MString& name);

        /**
         * Adds a matrix attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddMatrix(const MString& name);

        /**
         * Adds a matrix array attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddMatrixArray(const MString& name);

        /**
         * Adds a string attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddString(const MString& name, const MString& defaultValue = "");

        /**
         * Adds a string array attribute.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddStringArray(const MString& name);

        /**
         * Adds a typed object attribute.
         * @param name [in] - The attribute name.
         * @param typeID [in] - The type identifier of the object.
         * @return The created attribute is returned.
         */
        MObject AddTypedObject(const MString& name, const MTypeId& typeID);

        /**
         * Adds an object reference attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddObjectReference(const MString& name);

        /**
         * Adds an object reference array attribute.
         * @param name [in] - The attribute name.
         * @return The created attribute is returned.
         */
        MObject AddObjectReferenceArray(const MString& name);

        /**
         * Adds a compound attribute.
         * @param name [in] - The attribute name.
         * @param attributeCount [in] - The number of attributes pointed to by the attributes parameter.
         * @param attributes [in] - Pointer to an array of attributes to add.
         * @return The created attribute is returned.
         */
        MObject AddCompound(const MString& name, int attributeCount, const MObject* attributes);

        /**
         * Adds a compound attribute.
         * @param name [in] - The attribute name.
         * @param attributes [in] - List of attributes to add.
         * @return The created attribute is returned.
         */
        MObject AddCompound(const MString& name, const std::list<MObject>& attributes);

        /**
         * Adds a compound array attribute.
         * @param name [in] - The attribute name.
         * @param attributeCount [in] - The number of attributes pointed to by the attributes parameter.
         * @param attributes [in] - Pointer to an array of attributes to add.
         * @return The created attribute is returned.
         */
        MObject AddCompoundArray(const MString& name, int attributeCount, const MObject* attributes);

        /**
         * Adds a compound array attribute.
         * @param name [in] - The attribute name.
         * @param attributes [in] - List of attributes to add.
         * @return The created attribute is returned.
         */
        MObject AddCompoundArray(const MString& name, const std::list<MObject>& attributes);

        /**
         * Adds a pixel format attribute.
         * This is an enum with a predefined list of values.
         * @param name [in] - The attribute name.
         * @param defaultValue [in] - The default value.
         * @return The created attribute is returned.
         */
        MObject AddPixelFormat(const MString& name, short defaultValue = 0);

        /**
         * Indicates whether adding the attribute is allowed.
         * This should be set to false when creating child attributes that will be used in a compound attribute
         */
        bool add;

        /** Indicates whether created attributes should be connectable. Defaults to true */
        bool connectable;

        /** Indicates whether created attributes should be hidden. Defaults to false */
        bool hidden;

        /** Defaults to true */
        bool readable;

        /** Defaults to true */
        bool writable;

        /** Defaults to true */
        bool storable;

        /** Defaults to false */
        bool keyable;

        /**
         * The dependency node to which attributes are added.
         * If not set, attributes are added with the MPxNode::addAttribute() method
         */
        MObject object;

    private:
        void SetUp(MFnAttribute& attr);
        void AddAttribute(MObject& attr);
    };

} }

#Copyright (c) 2017 Finjin
#
#This file is part of Finjin Exporter (finjin-exporter).
#
#Finjin Exporter is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#Finjin Exporter is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with Finjin Exporter.  If not, see <http://www.gnu.org/licenses/>.


import os
import re

def getfiles(ext):
    result = []
    files = os.listdir('.')
    for f in files:
        if os.path.splitext(f)[1] == ext:
            result.append(f)
    return result            
    
if __name__ == '__main__':
    #Modify this to match the Maya installation you are using during development
    templatesdirectory = 'C:\\Program Files\\Autodesk\\Maya2011\\scripts\\AETemplates\\'
    
    scriptfiles = getfiles('.mel')

    regexp = re.compile('(AEFinjin|Finjin|Template|\.mel)')    

    for scriptfile in scriptfiles:
        outfilename =  'Install' + scriptfile + '.bat'
        outfilename = regexp.sub('', outfilename)
        
        outfile = open(outfilename, 'wt')
        outfile.write('copy %s "%s"\n' % (scriptfile, templatesdirectory))
        outfile.close()

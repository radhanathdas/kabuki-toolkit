/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/PathTrail.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

                      All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <Kabuki_Theater/Cell_IGA/PathTrail.h>

namespace Kabuki_Theater { namespace Cell_IGA {
    
PathTrail::PathTrail ()
{
    trailColor   = DefaultTrailColor;
    maxNumPoints = DefaultMaxNumPoints;
    
    initialize ();
}

PathTrail::PathTrail (Color thisTrailColor)
{
    if (thisTrailColor == null)
        trailColor = DefaultTrailColor;
    else
        trailColor = thisTrailColor;
        
    maxNumPoints = DefaultMaxNumPoints;
        
    initialize ();
}

PathTrail::PathTrail (Color thisTrailColor, int thisMaxNumPoints)
{
    if (thisTrailColor == null)
        trailColor = DefaultTrailColor;
    else
        trailColor = thisTrailColor;
        
    if (thisMaxNumPoints < 0)
        maxNumPoints = DefaultMaxNumPoints;
    else
        maxNumPoints = thisMaxNumPoints;
    
    initialize ();
}

void PathTrail::initialize ()
{
    startPoint = stopPoint = numPoints = 0;
    
    xpoints = new int[maxNumPoints];
    ypoints = new int[maxNumPoints];
}

void PathTrail::setTrailColor (Color newColor)
{
    if (newColor != null)
        trailColor = newColor;
}

Color PathTrail::getTrailColor ()
{
    return trailColor;
}

void PathTrail::draw (Graphics g)
{
    //System.out.print (toString ());
    g.setColor (trailColor);
    
    for (int i = startPoint; i < stopPoint-1; ++i)
    {
        if (i >= maxNumPoints)
             i = 0;
        
        g.drawLine (xpoints[i], ypoints[i], xpoints[i+1], ypoints [i+1]);
    }
    for (int i = 0; i <= stopPoint-2; ++i)
    {
        g.drawLine (xpoints[i], ypoints[i], xpoints[i+1], ypoints [i+1]);
    }
}

void PathTrail::addPoint (int x, int y)
{
    //System.out.print ("\nAdding point { x = " + x + ", y = " + y + " }\n");
    
    if (numPoints < maxNumPoints)
        ++numPoints;
    
    // This is a circular que. This means that when we add at first, the stopIndex is going to increment
    // until we hit the maxNumPoints, then the startIndex and stopIndex will increment and loop around such
    // that startIndex = stopIndex + 1.
    
    xpoints[stopPoint] = x;
    ypoints[stopPoint] = y;
    
    if (++stopPoint >= maxNumPoints)
        stopPoint = 0;
    
    if (stopPoint == startPoint)
        startPoint++;
        
    if (startPoint >= maxNumPoints)
        startPoint = 0;
    
    //System.out.print (toString ());
}

String PathTrail::toString ()
{
    String returnString = "PathTrail: numPoints = " + numPoints + ", startPoint = " + startPoint + ", stopPoint = " + stopPoint + "\n";
    
    for (int i = 0; i < numPoints; i++)
        returnString +=  i + ".) x = " + xpoints[i] + ", y = " + ypoints[i] + "\n";
    
    return returnString;
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {

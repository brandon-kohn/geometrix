//
//! Copyright © 2008-2009
//! Brandon Kohn
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef GENERATIVE_GEOMETRY_DOC_MAINPAGE_HPP
#define GENERATIVE_GEOMETRY_DOC_MAINPAGE_HPP
#pragma once

/*!
\defgroup Primitives Geometric Primitives
\defgroup PrimitiveTraits Geometric Primitive Traits
\defgroup Concepts Geometric Concepts
\defgroup PrimitiveConcepts Geometric Primitive Concepts
\defgroup CoordinateReferenceFrames Coordinate Reference Frames
\defgroup CoordinateTransformations Coordinate Transformations

\mainpage Generative Geometry Algorithms Library

\htmlonly
<hr>
<div class="blockquote"><blockquote class="blockquote">
<p>

</p>
<p>
&#8220;<span class="quote">Geometry existed before the creation.</span>&#8221;<br> <span class="bold"><strong><span class="emphasis"><em>--
Plato</em></span></strong></span>
</p>
<p>
</p>
</blockquote></div>
\endhtmlonly

\section description Description

Generative Geometry Algorithms (GGA) is a computational geometry algorithms library whose focus is the delivery of generative geometry algorithms for use in application development.

\htmlonly
<div class="toc">
<p><b>Table of Contents</b></p>
<dl>
<dt><span class="preface"><a href="index.html">Preface</a></span></dt>

<dd><dl>
<dt><span class="chapter"><a href="index.html#motivation">Motivation</a></span></dt>
<dt><span class="chapter"><a href="index.html#rationale">Rationale</a></span></dt>
</dl></dd>

<dt><span class="part"><a href="introduction.html#introduction">Introduction</a></span></dt>
<dd><dl>
<dt><span class="chapter"><a href="terminology.html">Terminology</a></span></dt>
<dt><span class="chapter"><a href="concepts.html">Concepts</a></span></dt>
<dd><dl>
<dt><span class="chapter"><a href="indexed_sequence.html">Indexed Sequence</a></span></dt>
<dt><span class="chapter"><a href="numeric_sequence.html">Numeric Sequence</a></span></dt>
<dt><span class="chapter"><a href="coordinate_sequence.html">Coordinate Sequence</a></span></dt>
</dl></dd>
<dt><span class="chapter"><a href="primitives.html">Geometric Primitives</a></span></dt>
</dl></dd>
<dt><span class="part"><a href="support.html">Support</a></span></dt>

</dl>
</div>
\endhtmlonly

\section motivation Motivation

Computational Geometry is a relatively new field arising from the wave of algorithms development in the 1960s. Much of the early research done on computational geometry has been theoretical in nature relying on exact precision arithmetic. Subsequent work has addressed these issues, but to date the large body of robust computational geometry algorithms has been in the domain of commercially backed library developers.

\section rationale Rationale

\subsection generative_rationale Trait Interface
Much of the library's design has come out of the desire to experiment with generative coding techniques and the need to create an interface layer between newly developed geometry algorithms and proprietary legacy geometry code. The algorithms had to be able to interact with geometry via proxy interfaces in as efficient manner as was practical. This design gave the freedom to work on geometric algorithms which could be used in the public domain as well as by proprietary (and legacy) codes. A traits based approach to the interface was decided to be the best way for the algorithms to interact with the geometry in a practical and efficient manner while preserving a layer of abstraction between accessing geometric data and how it is implemented. 

\subsection indexed_sequence_rationale Indexed Sequences

The primitive types in geometry all use some form of numeric sequence in some \f$N\f$-dimensional space (whose dimensionality is generally static.) These numeric sequences are used to define points and vectors which are in turn used to create more complex objects. During the discussions on the <a href="http://www.boost.org">Boost</a> newsgroup the notion of using compile-time access to the coordinates of these numeric sequences came up often. It was decided that such accesses should be in general coordinate agnostic and that a compile-time mechanism like that of <a href="http://www.boost.org/doc/libs/1_37_0/libs/tuple/doc/tuple_users_guide.html">Boost.Tuple</a> should be employed. Another camp of developers voiced concerns about working in higher dimensions and how compile-time accesses can actually degrade performance due to code-bloat. As a result of these discussions it was decided that for this library the means by which a numeric sequence is accessed should be decided by the users when the sequences are defined. From this the idea of the indexed_access_traits<T> type was formed along with the notions of how to define both run-time and compile time traversals in an efficient manner. These points are discussed further under the section \ref indexed_sequence.

\page introduction Introduction
\htmlonly
<div class="blockquote"><blockquote class="blockquote">
<p>

</p>
<p>
&#8220;<span class="quote">The cowboys have a way of trussing up a steer or a pugnacious bronco which fixes the brute so that it can neither move nor think. This is the hog-tie, and it is what Euclid did to geometry.</span>&#8221;<br> <span class="bold"><strong><span class="emphasis"><em>--
Eric T. Bell, author of The Search For Truth</em></span></strong></span>
</p>
<p>
</p>
</blockquote></div>
\endhtmlonly

Points, lines, segments, and polygons are in general abstract concepts which can be defined in many dimensions and with many different numerical types. Algorithms which operate on these conceptual types may also be abstracted in the same way that the STL generalizes the relationship between a container and the element contained. Thus the first job of a generative geometry library is to define a generic geometry type system which allows users to describe geometry in a sensibly generic way. The second job is to allow users to make use of their own existing legacy geometry types and algorithms in tandem. Both of these objectives are accomplished in GGA through the use of a traits based interface which allows users to define and specialize existing geometric types. Algorithms may then operate on these types by accessing required inputs through a specialized traits interface.\n
\n
The traits based geometry type system is easy to use and often may be created entirely from macros. Here is an example showing how to create a simple type configuration for points, segments, and polylines:

\code
// Here is a simple point structure to model a 3D point with type double.
struct point3D
{
    point3D( double x, double y, double z )
    {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }

    template <unsigned int Index>
    double&          get() { return coords[Index]; }
    template <unsigned int Index>
    const double&    get() const { return coords[Index]; }

    double coords[3];
};

// Here is a macro declaration that turns this simple struct into a GGA enabled point type with 
// a cartesian reference frame and a preference for compile time access semantics.
GENERATIVE_GEOMETRY_DEFINE_USER_POINT_TRAITS
(
    point3D,                             // The real type
    double,                              // The underlying coordinate type
    3,                                   // The dimensionality of the point type
    cartesian_reference_frame_double_3d, // The default reference frame
    prefer_compile_time_access_policy    // The preferred index access policy
);

// In order to provide a means to construct the points inside algorithms, the following macro 
// can be used to generate a construction_traits specialization for the type if the type has
// a default constructor which takes an enumerated list of coordinate values. (e.g. x, y, z ).
GENERATIVE_GEOMETRY_DEFINE_NUMERIC_SEQUENCE_CONSTRUCTION_TRAITS( point3D, 3 );
\endcode

The resulting point type can then be used in any algorithm which conforms to the point_traits interface.

For example:
\code

\endcode

\page terminology Terminology

The following terms are used in the library to describe various geometric types and concepts:

- <b>Affine Space</b> -- An \f$N\f$-dimensional space of point locations and the vector sub-space connecting them.
- <b>Coordinate Reference Frame</b> -- An affine space with a determined origin, a set of linearly independent basis vectors defining coordinate axes, and a set of metrics defining coordinate units.
- <b>Coordinate Transformation</b> -- A mapping algorithm which can redefine points in one frame of reference in terms of another frame of reference.
- <b>Point</b> -- A location in \f$N\f$-dimensional space.
- <b>Vector</b> -- A direction and magnitude in \f$N\f$-dimensional space. May also be defined as a translational mapping from one point in \f$N\f$-dimensional space to another.
- <b>Segment</b> -- A line segment in \f$N\f$-dimensional space consisting of a start point and an end point.
- <b>Point Sequence</b> -- An ordered sequence of points whose ordering has some significance with respect to the form of a geometric shape.
- <b>Polyline</b> -- A point sequence which defines a series of line segments which does not form a ring.
- <b>Polygon</b> -- A point sequence which defines a boundary region of some geometric shape.

\page concepts Concepts

- PointConcept
    - Point2DConcept
    - Point3DConcept
- VectorConcept
    - Vector2DConcept
    - Vector3DConcept
- SegmentConcept
- PointSequenceConcept

\page primitives Geometric Primitives

\page indexed_sequence Indexed Sequence

\section indexed_sequence_description Description

An indexed sequence is a \ref random_access_sequence without an explicit interface for iterators. The requirement for an indexed sequence is to provide constant time arbitrary access to elements through a compile-time or run-time specified index.

\section indexed_sequence_refinement Refinement of

\ref forward_sequence

\htmlonly
<div class="informaltable"><table class="table">
<colgroup>
<col>
<col>
<col>

<col>
</colgroup>
<thead><tr>
<th>
<p>
Expression
</p>
</th>
<th>
<p>
Return type
</p>

</th>
<th>
<p>
Type Requirements
</p>
</th>
<th>
<p>
Runtime Complexity
</p>
</th>

</tr></thead>
<tbody>
<tr>
<td>
<p>
<tt class="computeroutput"><a href="../intrinsic/functions/begin.html" title="begin"><tt class="computeroutput"><span class="identifier">begin</span></tt></a><span class="special">(</span><span class="identifier">s</span><span class="special">)</span></tt>
</p>
</td>
<td>

<p>
<a href="../../iterator/concepts/random_access_iterator.html" title="Random
Access Iterator">Random
Access Iterator</a>
</p>
</td>
<td>
<p>
</p>
</td>
<td>

<p>
Constant
</p>
</td>
</tr>
<tr>
<td>
<p>
<tt class="computeroutput"><a href="../intrinsic/functions/end.html" title="end"><tt class="computeroutput"><span class="identifier">end</span></tt></a><span class="special">(</span><span class="identifier">s</span><span class="special">)</span></tt>

</p>
</td>
<td>
<p>
<a href="../../iterator/concepts/random_access_iterator.html" title="Random
Access Iterator">Random
Access Iterator</a>
</p>
</td>
<td>
<p>

</p>
</td>
<td>
<p>
Constant
</p>
</td>
</tr>
<tr>
<td>
<p>

<tt class="computeroutput"><a href="../intrinsic/functions/at.html" title="at"><tt class="computeroutput"><span class="identifier">at</span></tt></a><span class="special">&lt;</span><span class="identifier">N</span><span class="special">&gt;(</span><span class="identifier">s</span><span class="special">)</span></tt>
</p>
</td>
<td>
<p>
Any type
</p>

</td>
<td>
<p>
</p>
</td>
<td>
<p>
Constant
</p>
</td>

</tr>
<tr>
<td>
<p>
<tt class="computeroutput"><a href="../intrinsic/functions/at.html" title="at"><tt class="computeroutput"><span class="identifier">at</span></tt></a><span class="special">&lt;</span><span class="identifier">N</span><span class="special">&gt;(</span><span class="identifier">s</span><span class="special">)</span>
<span class="special">=</span> <span class="identifier">o</span></tt>

</p>
</td>
<td>
<p>
Any type
</p>
</td>
<td>
<p>
<tt class="computeroutput"><span class="identifier">s</span></tt> is mutable and
<tt class="computeroutput"><span class="identifier">e</span> <span class="special">=</span>

<span class="identifier">o</span></tt>, where <tt class="computeroutput"><span class="identifier">e</span></tt> is the first element in the
sequence, is a valid expression.
</p>
</td>
<td>
<p>
Constant
</p>
</td>

</tr>
</tbody>
</table></div>
\endhtmlonly

\page support Support

Please direct all questions to GGA's mailing list. You can access the GGA Mailing List through http://sourceforge.net/mailarchive/forum.php?forum_name=gengeomalg-development . You may also access via Gmane at: news://news.gmane.org/gmane.comp.mathematics.gengeomalg.devel . The news group mirrors the mailing list. Here is a link to the archives: http://news.gmane.org/gmane.comp.mathematics.gengeomalg.devel 

*/

#endif //GENERATIVE_GEOMETRY_DOC_MAINPAGE_HPP

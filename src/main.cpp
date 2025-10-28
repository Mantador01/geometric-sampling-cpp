#include "sampling.h"
#include "svg.h"

#include <map>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

// Pour la derniere question avec la patate 3D
#include <queue>
#include <set>
#include <algorithm>
#include <functional>

//===== Test activation =====

#define ORIENT_TEST
#define HULL_POINT_TEST
#define WRAP_2D_TEST
#define HULL_2D_TEST
#define WRAP_3D_TEST
#define HULL_EDGE_TEST
#define HULL_3D_TEST

static constexpr double res = 100 ;
static constexpr double pt_rad = 2 ;

//===== Orientation predicate =====

int sign(double d) {
  if(d > 0) return  1 ;
  if(d < 0) return -1 ;
  return 0 ;
}

int orient(
    const Point& p0, 
    const Point& p1, 
    const Point& p2
    )
{
  Vector v0(p0) ;
  Vector v1(p1) ;
  Vector v2(p2) ;
  v0.z = 1 ;
  v1.z = 1 ;
  v2.z = 1 ;
  
  double d = dot(cross(v0,v1),v2) ;
  return sign(d) ;
}

int orient(
    const Point& p0, 
    const Point& p1, 
    const Point& p2, 
    const Point& p3
    )
{
  Vector v0(p0) ;
  Vector v1(p1) ;
  Vector v2(p2) ;
  Vector v3(p3) ;
  
  double d = dot(cross(v0,v1),v2) ;
  d -= dot(cross(v1,v2),v3) ;
  d += dot(cross(v2,v3),v0) ;
  d -= dot(cross(v3,v0),v1) ;
  return sign(d) ;
}

//===== Jarvis walk =====

//-- getting a point on the convex hull --


// Surcharge ==
inline bool operator==(const Point &a, const Point &b) {
  return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

// Surcharge !=
inline bool operator!=(const Point &a, const Point &b) {
  return !(a == b);
}

std::size_t hull_point(
    const std::vector<Point>& points
    )
{
  std::size_t index = 0;
    for (std::size_t i = 1; i < points.size(); ++i) {
        if (points[i](0) < points[index](0)) {
            index = i;
        }
    }
    return index;
}

//-- 2D wrapping

std::size_t wrap(
    const std::vector<Point>& points, 
    const Point& p0
    )
{
  // On boucle jusqu'à trouver l'index de p0 
  std::size_t index_p0 = 0;
  while (index_p0 < points.size() && points[index_p0] != p0)
    ++index_p0;

  // premier point différent de p0
  std::size_t index_p1 = (index_p0 == 0) ? 1 : 0;

  // On trouve le point le plus à droite
  for (std::size_t i = 0; i < points.size(); ++i) {
      // On ignore p0 et le candidat actuel
      if (i == index_p0 || i == index_p1)
          continue;

      // Si points[i] est à droite de la droite (p0, candidate p1)
      if (orient(p0, points[index_p1], points[i]) == -1) {
          index_p1 = i;
      }
  }

  return index_p1;
}

//-- 3D wrapping

std::size_t wrap(
    const std::vector<Point>& points, 
    const Point& p0,
    const Point& p1
    )
{
  // différent de p0 et p1
  std::size_t candidate = 0;
  // Si le premier point est égal à p0 ou p1, choisir un autre
  if (points[candidate] == p0 || points[candidate] == p1)
      candidate = 1;
  if (points[candidate] == p0 || points[candidate] == p1)
      candidate = 2; 

  // Parcourir tous les points pour améliorer le candidat.
  for (std::size_t i = 0; i < points.size(); ++i) {
      // Ignorer p0 et p1.
      if (points[i] == p0 || points[i] == p1)
          continue;

      // Si points[i] est "en dessous" du plan formé par p0, p1 et le candidat actuel...
      if (orient(p0, p1, points[candidate], points[i]) == -1) {
          candidate = i;
      }
  }
  return candidate;
}

//-- convex hull

void jarvis2d(
    const std::vector<Point>& points, 
    std::vector<std::size_t>& hull
    ) 
{
  // On vérifie qu'il y a au moins un point
  if(points.empty())
    return;

  // le point le plus à gauche
  std::size_t start = hull_point(points);
  std::size_t current = start;

  // on ajoute le point courant et on cherche le point suivant
  do {
    hull.push_back(current);
    std::size_t next = wrap(points, points[current]);
    current = next;
  } while(current != start);
}



// LA PATATE 3D 

struct EdgeData {
  std::size_t a, b, opp;
};

using edge = std::pair<std::size_t, std::size_t>;

// Comparateur pour les arêtes
struct EdgeCompare {

  bool operator()(const edge &e1, const edge &e2) const {

    std::size_t min1 = std::min(e1.first, e1.second);
    std::size_t min2 = std::min(e2.first, e2.second);

    if (min1 != min2)
      return min1 < min2;

    std::size_t max1 = std::max(e1.first, e1.second);
    std::size_t max2 = std::max(e2.first, e2.second);

    return max1 < max2;
  }
};

void jarvis3d(
    const std::vector<Point>& points, 
    std::vector<std::size_t>& hull
    ) 
{
  if(points.size() < 3)
    return;


  std::size_t p0 = hull_point(points);
  std::size_t p1 = (p0 == 0) ? 1 : 0;
  std::size_t p2 = wrap(points, points[p0], points[p1]);
  
  hull.push_back(p0);
  hull.push_back(p1);
  hull.push_back(p2);

  std::set<edge, EdgeCompare> processedEdges;



  std::function<void(const EdgeData&)> processEdge = [&](const EdgeData &ed) {
    edge ekey = {std::min(ed.a, ed.b), std::max(ed.a, ed.b)};
    if (processedEdges.find(ekey) != processedEdges.end())
      return;
  
    
    processedEdges.insert(ekey);

    std::size_t candidate = wrap(points, points[ed.a], points[ed.b]);

    if(candidate == ed.opp) {
      std::size_t candidate2 = wrap(points, points[ed.b], points[ed.a]);
      if(candidate2 != ed.opp) {

        hull.push_back(ed.b);
        hull.push_back(ed.a);
        hull.push_back(candidate2);

        processEdge({ed.a, candidate2, ed.b});
        processEdge({candidate2, ed.b, ed.a});
      }
      return;
    }

    // si le candidat est le même que l'opposé, on ne fait rien
    hull.push_back(ed.a);
    hull.push_back(ed.b);
    hull.push_back(candidate);


    processEdge({ed.b, candidate, ed.a});
    processEdge({candidate, ed.a, ed.b});
  };

  processEdge({p0, p1, p2});
  processEdge({p1, p2, p0});
  processEdge({p2, p0, p1});
}

int main() {

  {
    //2D points for 2D tests
    
    std::vector<Point> points ;

    //100 random points in the unit disk
    for(int i = 0; i < 100; ++i) {
      points.push_back(rand_in_disk()) ;
    }

#ifdef ORIENT_TEST
    {
      //test orientation

      //segment
      Point s0 = {-1,-1, 0} ;
      Point s1 = { 1, 1, 0} ;

      //plot the input points
      SVG::SVG svg ;
      SVG::Element* grp = svg.add_child<SVG::Group>() ;
      for(Point& pt : points) {
        grp->add_child<SVG::Circle>(res * pt(0), -res * pt(1), pt_rad) ;
      }

      //plot the separating line
      SVG::Line* segment = grp->add_child<SVG::Line>(
          res  * s0(0), 
          res  * s1(0), 
          -res * s0(1), 
          -res * s1(1)
          ) ;

      segment->set_attr("stroke", "black") ;

      //color them according to their side wrt. the line between the extremes
      for(Point& pt : points) {
        SVG::Circle* c = grp->add_child<SVG::Circle>(res * pt(0), -res * pt(1), 2 * pt_rad) ;
        c->set_attr("fill", "none") ;
        int o = orient(s0, s1, pt) ;
        if(o > 0) {
          c->set_attr("stroke", "red") ;
        } else {
          c->set_attr("stroke", "green") ;
        }
      }

      //export the final svg
      svg.autoscale() ;
      std::ofstream file("orient.svg") ;
      file << std::string(svg) ;

      std::cout << "orientation test generated in orient.svg" << std::endl ;
    }
#endif

#ifdef HULL_POINT_TEST
    {
      //test hull point

      //plot the input points
      SVG::SVG svg ;
      SVG::Element* grp = svg.add_child<SVG::Group>() ;
      for(Point& pt : points) {
        grp->add_child<SVG::Circle>(res * pt(0), -res * pt(1), pt_rad) ;
      }

      //plot the hull point
      std::size_t l = hull_point(points) ;
      SVG::Circle* c = grp->add_child<SVG::Circle>(res * points[l](0), -res * points[l](1), 2 * pt_rad) ;
      c->set_attr("fill", "none") ;
      c->set_attr("stroke", "black") ;

      //export the final svg
      svg.autoscale() ;
      std::ofstream file("hull_point.svg") ;
      file << std::string(svg) ;

      std::cout << "hull point test generated hull_point.svg" << std::endl ;
    }
#endif

#ifdef WRAP_2D_TEST
    {
      //test wrap

      //plot the input points
      SVG::SVG svg ;
      SVG::Element* grp = svg.add_child<SVG::Group>() ;
      for(Point& pt : points) {
        grp->add_child<SVG::Circle>(res * pt(0), -res * pt(1), pt_rad) ;
      }

      //plot the hull point
      std::size_t l = hull_point(points) ;
      SVG::Circle* cl = grp->add_child<SVG::Circle>(res * points[l](0), -res * points[l](1), 2 * pt_rad) ;
      cl->set_attr("fill", "none") ;
      cl->set_attr("stroke", "black") ;

      //plot the next point obtained through wrapping
      std::size_t n = wrap(points, points[l]) ;
      SVG::Circle* cn = grp->add_child<SVG::Circle>(res * points[n](0), -res * points[n](1), 2 * pt_rad) ;
      cn->set_attr("fill", "none") ;
      cn->set_attr("stroke", "blue") ;

      //plot the hull segment
      SVG::Line* segment = grp->add_child<SVG::Line>(
          res * points[l](0), 
          res * points[n](0), 
          -res * points[l](1), 
          -res * points[n](1)
          ) ;

      segment->set_attr("stroke", "black") ;


      //export the final svg
      svg.autoscale() ;
      std::ofstream file("wrap.svg") ;
      file << std::string(svg) ;

      std::cout << "2D wrap test generated wrap_2d.svg" << std::endl ;
    }
#endif

#ifdef HULL_2D_TEST
    {
      //test hull 2d

      //compute the convex hull
      std::vector<std::size_t> hull ;
      //gw_plot.toggle() ;
      jarvis2d(points, hull) ;
      //gw_plot.toggle() ;

      //plot the input points
      SVG::SVG svg ;
      SVG::Element* grp = svg.add_child<SVG::Group>() ;
      for(Point& pt : points) {
        grp->add_child<SVG::Circle>(res * pt(0), -res * pt(1), pt_rad) ;
      }

      //plot the hull polygon
      std::vector<SVG::Point> svgpts ;
      for(std::size_t& i : hull) {
        SVG::Circle* c = grp->add_child<SVG::Circle>(res * points[i](0), -res * points[i](1), 2 * pt_rad) ;
        c->set_attr("fill", "none") ;
        c->set_attr("stroke", "black") ;
        svgpts.push_back({res * points[i](0), -res * points[i](1)}) ;
      }
      
      SVG::Polygon* poly = grp->add_child<SVG::Polygon>(svgpts) ;
      poly->set_attr("fill", "none") ;
      poly->set_attr("stroke", "black") ;

      //export the final svg
      svg.autoscale() ;
      std::ofstream file("hull_2d.svg") ;
      file << std::string(svg) ;

      std::cout << "2D hull generated in hull_2d.svg" << std::endl ;
    }
#endif
  }


  {
    //3D points for 3D tests
    
    std::vector<Point> points ;

    //100 random points in the unit disk
    for(int i = 0; i < 100; ++i) {
      points.push_back(rand_in_ball()) ;
    }

#ifdef WRAP_3D_TEST
    {
      //test hull 3d

      //export to wavefront obj file
      std::ofstream file("wrap_3d.obj") ;

      //vertex export
      for(const Point& pt : points) {
        file << "v " << pt(0) << " " << pt(1) << " " << pt(2) << "\n" ;
      }

      //hull point p0
      Point p0 = {-1,-1,-1} ;
      Point p1 = {-1, 1,-1} ;

      file << "v " << p0(0) << " " << p0(1) << " " << p0(2) << "\n" ; 
      file << "v " << p1(0) << " " << p1(1) << " " << p1(2) << "\n" ; 

      //wrapped point p2
      std::size_t p_index = wrap(points, p0, p1) ;

      //initial triangle export
      file << "f " << points.size() + 1 << " " << p_index + 1 << " " <<  points.size() + 2 << "\n" ;
      file.flush() ;

      std::cout << "3D wrap test generated in wrap_3d.obj" << std::endl ;
    }
#endif

#ifdef HULL_EDGE_TEST
    {
      //test hull 3d

      //export to wavefront obj file
      std::ofstream file("hull_edge.obj") ;

      //vertex export
      for(const Point& pt : points) {
        file << "v " << pt(0) << " " << pt(1) << " " << pt(2) << "\n" ;
      }

      //hull point p0
      std::size_t p0_index = hull_point(points) ;

      //additional point q
      Point q = points[p0_index] + Point(0,1,0) ;

      //wrapped point p1
      std::size_t p1_index = wrap(points, points[p0_index], q) ;

      //medium point to draw edge as a degenerate triangle
      Point mid = 0.5*(points[p0_index] + points[p1_index]) ;
      file << "v " << mid(0) << " " << mid(1) << " " << mid(2) << "\n" ; 

      //initial triangle export
      file << "f " << p0_index + 1 << " " << points.size() + 1 << " " << p1_index + 1 << "\n" ;
      file.flush() ;

      std::cout << "3D hull_edge test generated in hull_edge.obj" << std::endl ;
    }
#endif

#ifdef HULL_3D_TEST
    {
      //test hull 3d

      //compute the convex hull
      std::vector<std::size_t> hull ;
      jarvis3d(points, hull) ;

      //export to wavefront obj file
      std::ofstream file("hull_3d.obj") ;

      //vertex export
      for(const Point& pt : points) {
        file << "v " << pt(0) << " " << pt(1) << " " << pt(2) << "\n" ;
      }

      //triangle export
      for(std::size_t i = 0; i < hull.size(); i += 3) {
        file << "f " << hull[i]+1 << " " << hull[i+1]+1 << " " << hull[i+2]+1 << "\n" ;
      }
      file.flush() ;

      std::cout << "3D hull generated in hull_3d.obj" << std::endl ;
    }
#endif
  }

  return 0 ;
}

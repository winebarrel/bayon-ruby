//
// Utilities for vector operation
//
// Copyright(C) 2009  Mizuki Fujisawa <fujisawa@bayon.cc>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#include "byvector.h"

namespace bayon {

/* Get items sorted by value */
void Vector::sorted_items(std::vector<VecItem> &items) const {
  for (VecHashMap::const_iterator it = vec_.begin(); it != vec_.end(); ++it) {
    VecItem item;
    item.first = it->first;
    item.second = it->second;
    items.push_back(item);
  }
  std::sort(items.begin(), items.end(), greater_pair<VecKey, VecValue>);
}

/* Get items sorted by absolute value */
void Vector::sorted_items_abs(std::vector<VecItem> &items) const {
  for (VecHashMap::const_iterator it = vec_.begin(); it != vec_.end(); ++it) {
    VecItem item;
    item.first = it->first;
    item.second = it->second;
    items.push_back(item);
  }
  std::sort(items.begin(), items.end(), greater_pair_abs<VecKey, VecValue>);
}

/* Normalize the vector */
void Vector::normalize() {
  double nrm = norm();
  for (VecHashMap::iterator it = vec_.begin(); it != vec_.end(); ++it) {
    double point = it->second / nrm;
    vec_[it->first] = point;
  }
}

/* Resize the vector */
void Vector::resize(size_t size) {
  if (vec_.size() <= size) return;
  std::vector<VecItem> items;
  sorted_items_abs(items);
#ifdef HAVE_GOOGLE_DENSE_HASH_MAP
  vec_.set_deleted_key(VECTOR_DELETED_KEY);
#endif
  for (size_t i = size; i < items.size(); i++) {
    vec_.erase(items[i].first);
  }
#ifdef HAVE_GOOGLE_DENSE_HASH_MAP
  vec_.clear_deleted_key();
#endif
}

/* Calculate squared norm of the vector */
double Vector::norm_squared() const {
  double sum = 0;
  for (VecHashMap::const_iterator it = vec_.begin(); it != vec_.end(); ++it) {
    double point = it->second;
    sum += point * point;
  }
  return sum;
}

/* Calculate norm of the vector */
double Vector::norm() const {
  return sqrt(norm_squared());
}

/* Multiply each value of vector by constant */
void Vector::multiply_constant(double x) {
  for (VecHashMap::iterator it = vec_.begin(); it != vec_.end(); ++it) {
    double point = it->second * x;
    vec_[it->first] = point;
  }
}

/* Add other vector */
void Vector::add_vector(const Vector &vec) {
  for (VecHashMap::const_iterator it = vec.hash_map()->begin();
       it != vec.hash_map()->end(); ++it) {
    vec_[it->first] += it->second;
  }
}

/* Delete other vector */
void Vector::delete_vector(const Vector &vec) {
  for (VecHashMap::const_iterator it = vec.hash_map()->begin();
       it != vec.hash_map()->end(); ++it) {
    vec_[it->first] -= it->second;
  }
}

/* Calculate squared euclid distance between vectors */
double Vector::euclid_distance_squared(const Vector &vec1, const Vector &vec2) {
  HashMap<VecKey, bool>::type done;
  init_hash_map(VECTOR_EMPTY_KEY, done);
  VecHashMap::const_iterator it1, it2;
  double dist = 0;

  for (it1 = vec1.hash_map()->begin(); it1 != vec1.hash_map()->end(); ++it1) {
    double val = vec2.get(it1->first);
    dist += (it1->second - val) * (it1->second - val);
    done[it1->first] = true;
  }
  for (it2 = vec2.hash_map()->begin(); it2 != vec2.hash_map()->end(); ++it2) {
    if (done.find(it2->first) == done.end()) {
      double val = vec1.get(it2->first);
      dist += (it2->second - val) * (it2->second - val);
    }
  }
  return dist;
}

/* Calculate euclid distance between vectors */
double Vector::euclid_distance(const Vector &vec1, const Vector &vec2) {
  return sqrt(Vector::euclid_distance_squared(vec1, vec2));
}

/* Calculate inner product value between vectors */
double Vector::inner_product(const Vector &vec1, const Vector &vec2) {
  VecHashMap::const_iterator it, itother, end;
  const Vector *other;
  if (vec1.size() < vec2.size()) {
    it = vec1.hash_map()->begin();
    end = vec1.hash_map()->end();
    other = &vec2;
  } else {
    it = vec2.hash_map()->begin();
    end = vec2.hash_map()->end();
    other = &vec1;
  }

  double prod = 0;
  while (it != end) {
    itother = other->hash_map()->find(it->first);
    if (itother != other->hash_map()->end()) {
      prod += it->second * itother->second;
    }
    ++it;
  }
  return prod;
}

/* Calculate cosine value between vectors */
double Vector::cosine(const Vector &vec1, const Vector &vec2) {
  double norm1 = vec1.norm();
  double norm2 = vec2.norm();
  double result = 0.0;
  if (!norm1 && !norm2) {
    return result;
  } else {
    double prod = Vector::inner_product(vec1, vec2);
    result = prod / (norm1 * norm2);
    return isnan(result) ? 0.0 : result;
  }
}

/* Calculate Jaccard coefficient value between vectors */
double Vector::jaccard(const Vector &vec1, const Vector &vec2) {
  double norm1 = vec1.norm();
  double norm2 = vec2.norm();
  double prod = Vector::inner_product(vec1, vec2);
  double denom = norm1 + norm2 - prod;
  double result = 0.0;
  if (!denom) {
    return result;
  } else {
    result = prod / denom;
    return isnan(result) ? 0.0 : result;
  }
}

} /* namespace bayon */

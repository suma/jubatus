// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include <pficommon/lang/cast.h>
#include <pficommon/text/json.h>

#include "jubatus/core/storage/storage_type.hpp"
#include "jubatus/core/storage/local_storage_mixture.hpp"
#include "jubatus/core/classifier/classifier_test_util.hpp"
#include "jubatus/core/classifier/classifier.hpp"
#include "jubatus/core/fv_converter/datum.hpp"
#include "classifier.hpp"

#include "test_util.hpp"

using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::isfinite;
using std::numeric_limits;
using std::cout;
using std::endl;

using pfi::lang::lexical_cast;
using pfi::data::optional;
using jubatus::core::classifier::classify_result;
using jubatus::core::fv_converter::datum;

using jubatus::core::classifier::multiclass_classifier;
using jubatus::core::classifier::classifier_storage_ptr;

namespace jubatus {
namespace driver {

namespace {
datum convert_vector(const vector<double>& vec) {
  datum d;
  for (size_t i = 0; i < vec.size(); i++) {
    string f = "f" + lexical_cast<string>(i);
    d.num_values_.push_back(make_pair(f, vec[i]));
  }
  return d;
}

void make_random_data(
    pfi::math::random::mtrand& rand,
    vector<pair<string, datum> >& data,
    size_t size) {
  for (size_t i = 0; i < size; ++i) {
    pair<string, vector<double> > p = gen_random_data(rand);
    data.push_back(make_pair(p.first, convert_vector(p.second)));
  }
}

string get_max_label(const classify_result& result) {
  string max_label = "";
  double max_prob = 0;
  for (size_t i = 0; i < result.size(); ++i) {
    if (max_label == "" || result[i].score > max_prob) {
      max_label = result[i].label;
      max_prob = result[i].score;
    }
  }
  return max_label;
}
}  // namespace

class classifier_test : public ::testing::TestWithParam<multiclass_classifier*> {
 protected:
  void SetUp() {
    classifier_.reset(new driver::classifier(
          pfi::lang::shared_ptr<core::classifier::multiclass_classifier>(GetParam()),
          make_fv_converter()));
  }

  void TearDown() {
    classifier_.reset();
  }

  void my_test();

  pfi::lang::shared_ptr<classifier> classifier_;
};

TEST_P(classifier_test, simple) {
  datum d;
  classifier_->train(string("hoge"), d);
  classifier_->classify(d);
}

TEST_P(classifier_test, api_train) {
  pfi::math::random::mtrand rand(0);
  const size_t example_size = 1000;

  vector<pair<string, datum> > data;
  make_random_data(rand, data, example_size);
  for (size_t i = 0; i < example_size; i++) {
    classifier_->train(data[i].first, data[i].second);
  }
}

void classifier_test::my_test() {
  pfi::math::random::mtrand rand(0);
  const size_t example_size = 1000;

  vector<pair<string, datum> > data;
  make_random_data(rand, data, example_size);
  for (size_t i = 0; i < example_size; i++) {
    classifier_->train(data[i].first, data[i].second);
  }

  vector<string> labels;
  vector<classify_result> result;
  {
    vector<pair<string, datum> >::const_iterator it;
    for (it = data.begin(); it != data.end(); ++it) {
      labels.push_back(it->first);
      result.push_back(classifier_->classify(it->second));
    }
  }

  ASSERT_EQ(example_size, result.size());
  ASSERT_EQ(data.size(), result.size());

  vector<string>::const_iterator it0;  // answers
  vector<classify_result>::const_iterator it;
  size_t count = 0;
  for (it = result.begin(), it0 = labels.begin();
      it != result.end() && it0 != labels.end(); ++it, ++it0) {
    ASSERT_EQ(2u, it->size());  // estimate_results should have two label OK/NG
    string most0;
    double prob0 = DBL_MIN;
    classify_result::const_iterator ite;
    for (ite = it->begin(); ite != it->end(); ++ite) {
      // get most likely label
      if (prob0 < ite->score || ite == it->begin()) {
        prob0 = ite->score;
        most0 = ite->label;
      }
    }
    if (most0.compare(*it0) == 0) {
      count++;
    }
    // EXPECT_TRUE(*it0 == most0);
    if (most0.compare(*it0) != 0) {
      cout << *it0 << "!=" << most0 << endl;
      for (ite = it->begin(); ite != it->end(); ++ite) {
        cout << ite->label << "\t" << ite->score << endl;
      }
    }
  }
  // num of wrong classification should be less than 5%
  EXPECT_GE(count, result.size() - 50);
}

TEST_P(classifier_test, my_test) {
  my_test();
}

TEST_P(classifier_test, duplicated_keys) {
  pfi::math::random::mtrand rand(0);
  datum d;
  for (size_t k = 0; k < 10; ++k) {
    uint32_t dim = rand.next_int(100);
    pair<string, double> feature = make_pair(lexical_cast<string>(dim), 1.0);
    // add 100 duplicated values
    for (size_t j = 0; j < 100; ++j)
    d.num_values_.push_back(feature);
  }

  for (size_t i = 0; i < 100; ++i) {
    string label = i % 2 == 0 ? "P" : "N";
    classifier_->train(label, d);
  }

  {
    datum d;
    for (size_t i = 0; i < 100; ++i) {
      d.num_values_.push_back(make_pair(lexical_cast<string>(i), 1.0));
    }
    classify_result result = classifier_->classify(d);
    /* if the classifier could not learn properly, it estimates
       scores of labels to NaN and returns no results. */
    ASSERT_EQ(2u, result.size());
  }
}

TEST_P(classifier_test, save_load) {
  pfi::math::random::mtrand rand(0);
  const size_t example_size = 1000;

  vector<pair<string, datum> > data;
  make_random_data(rand, data, example_size);
  for (size_t i = 0; i < example_size; i++) {
    classifier_->train(data[i].first, data[i].second);
  }

  msgpack::sbuffer sbuf;
  core::framework::stream_writer<msgpack::sbuffer> swriter(sbuf);
  classifier_->save(swriter);

  classifier_->clear();

  msgpack::unpacked msg;
  msgpack::unpack(&msg, sbuf.data(), sbuf.size());
  classifier_->load(msg.get());

  my_test();
}

TEST_P(classifier_test, save_load_2) {
  msgpack::sbuffer save_empty, save_test;
  core::framework::stream_writer<msgpack::sbuffer>
    empty_writer(save_empty), test_writer(save_test);

  // Test data
  datum pos;
  pos.num_values_.push_back(make_pair("value", 10.0));
  datum neg;
  neg.num_values_.push_back(make_pair("value", -10.0));

  // Save empty state
  classifier_->save(empty_writer);

  // Train
  vector<pair<string, datum> > data;
  classifier_->train(string("pos"), pos);
  classifier_->train(string("neg"), neg);

  // Now, the classifier can classify properly
  ASSERT_EQ("pos", get_max_label(classifier_->classify(pos)));
  ASSERT_EQ("neg", get_max_label(classifier_->classify(neg)));

  // Save current state
  classifier_->save(test_writer);

  // Load empty
  {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, save_empty.data(), save_empty.size());
    classifier_->load(msg.get());
  }

  // And the classifier classify data improperly, but cannot expect results
  string pos_max = get_max_label(classifier_->classify(pos));
  string neg_max = get_max_label(classifier_->classify(neg));
  ASSERT_EQ(0, pos_max.compare(neg_max));

  // Reload server
  {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, save_test.data(), save_test.size());
    classifier_->load(msg.get());
  }

  // The classifier works well
  ASSERT_EQ("pos", get_max_label(classifier_->classify(pos)));
  ASSERT_EQ("neg", get_max_label(classifier_->classify(neg)));
}

TEST_P(classifier_test, nan) {
  datum d;
  d.num_values_.push_back(
      make_pair("value", numeric_limits<float>::quiet_NaN()));
  classifier_->train(string("l1"), d);


  classify_result result = classifier_->classify(d);
  ASSERT_EQ(1u, result.size());
  EXPECT_FALSE(isfinite(result[0].score));
}

vector<multiclass_classifier*> create_classifiers() {
  vector<multiclass_classifier*> method;
  classifier_storage_ptr storage;
  core::classifier::classifier_config config;

  // TODO(unknown): testing with perceptron?

  storage.reset(new core::storage::local_storage_mixture);
  method.push_back(
        new core::classifier::passive_aggressive(storage));

  storage.reset(new core::storage::local_storage_mixture);
  method.push_back(
        new core::classifier::passive_aggressive_1(config, storage));

  storage.reset(new core::storage::local_storage_mixture);
  method.push_back(
        new core::classifier::passive_aggressive_2(config, storage));

  storage.reset(new core::storage::local_storage_mixture);
  config.C = 0.1f;
  method.push_back(
        new core::classifier::normal_herd(config, storage));

  return method;
}

INSTANTIATE_TEST_CASE_P(classifier_test_instance,
    classifier_test,
    testing::ValuesIn(create_classifiers()));


}  // driver namespace
}  // jubatus namespace

#include <string>
#include <vector>
#include <map>

#include <bayon/cluster.h>

#include "bayonext_internal.h"

namespace {

class CBayonDocument {
  static VALUE rb_cBayonDocument;

  bayon::Document* document_;

  static void free(CBayonDocument *p) {
    if (p->document_) {
      delete p->document_;
    }

    delete p;
  }

  static VALUE alloc(VALUE klass) {
    CBayonDocument *p;

    p = new CBayonDocument;
    p->document_ = 0;

    return Data_Wrap_Struct(klass, 0, &free, p);
  }

  static VALUE initialize(VALUE self, VALUE v_docid) {
    CBayonDocument *p;

    long doc_id = NUM2LONG(v_docid);

    Data_Get_Struct(self, CBayonDocument, p);
    p->document_ = new bayon::Document(doc_id);

    return Qnil;
  }

  static VALUE add_feature(VALUE self, VALUE v_key, VALUE v_value) {
    CBayonDocument *p;

    long key = NUM2LONG(v_key);
    long value = NUM2LONG(v_value);

    Data_Get_Struct(self, CBayonDocument, p);
    p->document_->add_feature(key, value);

    return Qnil;
  }

public:
  static void check_type(VALUE obj) {
    const char * const classname = rb_class2name(CLASS_OF(obj));

    if (!rb_obj_is_instance_of(obj, rb_cBayonDocument)) {
      rb_raise(rb_eTypeError, "wrong argument type %s (expected Bayon::Document)", classname); \
    }
  }

  static bayon::Document* to_document(VALUE obj) {
    CBayonDocument *p;

    check_type(obj);
    Data_Get_Struct(obj, CBayonDocument, p);

    return p->document_;
  }

  static void init(VALUE &module) {
    rb_cBayonDocument = rb_define_class_under(module, "Document", rb_cObject);

    rb_define_alloc_func(rb_cBayonDocument, &alloc);
    rb_define_method(rb_cBayonDocument, "initialize", __F(&initialize), 1);
    rb_define_method(rb_cBayonDocument, "add_feature", __F(&add_feature), 2);
  }
};

VALUE CBayonDocument::rb_cBayonDocument = Qnil;

class CBayonAnalyzer {
  bayon::Analyzer* analyzer_;

  bool output_similairty_point_;

  static void free(CBayonAnalyzer *p) {
    if (p->analyzer_) {
      delete p->analyzer_;
    }

    delete p;
  }

  static VALUE alloc(VALUE klass) {
    CBayonAnalyzer *p;

    p = new CBayonAnalyzer;
    p->analyzer_ = 0;

    return Data_Wrap_Struct(klass, 0, &free, p);
  }

  static VALUE initialize(VALUE self) {
    CBayonAnalyzer *p;

    Data_Get_Struct(self, CBayonAnalyzer, p);
    p->analyzer_ = new bayon::Analyzer;
    p->output_similairty_point_ = false;

    return Qnil;
  }

  static VALUE add_document(VALUE self, VALUE v_doc) {
    CBayonAnalyzer *p;
    bayon::Document *document;

    document = CBayonDocument::to_document(v_doc);
    Data_Get_Struct(self, CBayonAnalyzer, p);
    p->analyzer_->add_document(*document);

    return Qnil;
  }

  static VALUE set_cluster_size_limit(VALUE self, VALUE v_limit) {
    CBayonAnalyzer *p;

    size_t limit = NUM2LONG(v_limit);

    Data_Get_Struct(self, CBayonAnalyzer, p);
    p->analyzer_->set_cluster_size_limit(limit);

    return Qnil;
  }

  static VALUE set_eval_limit(VALUE self, VALUE v_limit) {
    CBayonAnalyzer *p;

    double limit = NUM2DBL(v_limit);

    Data_Get_Struct(self, CBayonAnalyzer, p);
    p->analyzer_->set_eval_limit(limit);

    return Qnil;
  }

  static VALUE set_output_similairty_point(VALUE self, VALUE v_output) {
    CBayonAnalyzer *p;
    bool output = false;

    if (TYPE(v_output) == T_TRUE) {
      output = true;
    } else if (TYPE(v_output) == T_FALSE) {
      output = false;
    } else {
      rb_raise(rb_eTypeError, "wrong argument type %s (expected boolean value)");
    }

    Data_Get_Struct(self, CBayonAnalyzer, p);
    p->output_similairty_point_ = output;

    return Qnil;
  }

  static VALUE do_clustering(VALUE self, VALUE v_method) {
    CBayonAnalyzer *p;

    Check_Type(v_method, T_STRING);

    Data_Get_Struct(self, CBayonAnalyzer, p);
    p->analyzer_->do_clustering(std::string(RSTRING_PTR(v_method)));

    return Qnil;
  }

  static VALUE get_next_result(VALUE self) {
    typedef std::vector< std::pair<bayon::Document *, double> > documents;
    CBayonAnalyzer *p;
    bayon::Cluster cluster;

    Data_Get_Struct(self, CBayonAnalyzer, p);

    if(p->analyzer_->get_next_result(cluster)) {
      std::vector< std::pair<bayon::Document *, double> > pairs;
      cluster.sorted_documents(pairs);
      VALUE docids = rb_ary_new2(pairs.size());

      for(documents::const_iterator i = pairs.begin(); i != pairs.end(); i++) {
        bayon::Document* doc = i->first;
        double point = i->second;

        if (p->output_similairty_point_) {
          VALUE docid_points = rb_ary_new3(2, LONG2NUM(doc->id()), DBL2NUM(point));
          rb_ary_push(docids, docid_points);
        } else {
          rb_ary_push(docids, LONG2NUM(doc->id()));
        }
      }

      return docids;
    } else {
      return Qnil;
    }
  }

public:
  static void init(VALUE &module) {
    VALUE rb_cBayonAnalyzer = rb_define_class_under(module, "Analyzer", rb_cObject);

    rb_define_alloc_func(rb_cBayonAnalyzer, &alloc);
    rb_define_method(rb_cBayonAnalyzer, "initialize", __F(&initialize), 0);
    rb_define_method(rb_cBayonAnalyzer, "add_document", __F(&add_document), 1);
    rb_define_method(rb_cBayonAnalyzer, "set_cluster_size_limit", __F(&set_cluster_size_limit), 1);
    rb_define_method(rb_cBayonAnalyzer, "set_eval_limit", __F(&set_eval_limit), 1);
    rb_define_method(rb_cBayonAnalyzer, "set_output_similairty_point", __F(&set_output_similairty_point), 1);
    rb_define_method(rb_cBayonAnalyzer, "do_clustering", __F(&do_clustering), 1);
    rb_define_method(rb_cBayonAnalyzer, "get_next_result", __F(&get_next_result), 0);

    rb_define_const(rb_cBayonAnalyzer, "KMEANS", rb_str_new2("kmeans"));
    rb_define_const(rb_cBayonAnalyzer, "REPEATED_BISECTION", rb_str_new2("rb"));
  }
};

}

void Init_bayonext() {
  VALUE rb_mBayon;

  rb_mBayon = rb_define_module("Bayon");
  CBayonDocument::init(rb_mBayon);
  CBayonAnalyzer::init(rb_mBayon);
}

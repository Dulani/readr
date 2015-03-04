#ifndef FASTREAD_COLLECTORDOUBLE_H
#define FASTREAD_COLLECTORDOUBLE_H

#include "HCollector.h"
#include <Rcpp.h>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

class CollectorDouble : public Collector {
  Rcpp::NumericVector column_;

public:
  CollectorDouble(): column_(0) {
  }

  void resize(int n) {
    column_ = Rf_lengthgets(column_, n);
  }

  void setValue(int i, const Token& t) {
    column_[i] = parse(t);
  }

  double parse(const Token& t) {
    double res = 0.0;

    switch(t.type()) {
    case TOKEN_STRING: {
      std::string buffer;
      StreamIterators string = t.getString(&buffer);
      boost::spirit::qi::parse(string.first, string.second,
        boost::spirit::qi::double_, res);
      break;
    };
    case TOKEN_MISSING:
    case TOKEN_EMPTY:
      res = NA_REAL;
      break;
    case TOKEN_EOF:
      Rcpp::stop("Invalid token");
    }

    return res;
  }

  SEXP vector() {
    return column_;
  }

};

#endif
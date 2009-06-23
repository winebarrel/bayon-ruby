require 'bayonext'

module Bayon
  class Documents
    def initialize
      @documents = []
      @cluster_size_limit = nil
      @eval_limit = nil
      @output_similairty_point = nil
    end

    def cluster_size_limit=(limit)
      unless limit.kind_of?(Integer)
        raise TypeError, "wrong argument type #{limit.class} (expected Integer)"
      end

      @cluster_size_limit = limit
    end

    def eval_limit=(limit)
      unless limit.kind_of?(Numeric)
        raise TypeError, "wrong argument type #{limit.class} (expected Numeric)"
      end

      @eval_limit = limit
    end

    def output_similairty_point=(output)
      unless output.instance_of?(TrueClass) or output.instance_of?(FalseClass)
        raise TypeError, "wrong argument type #{limit.class} (expected boolean value)"
      end

      @output_similairty_point = output
    end
    
    def add_document(label, features)
      unless features.kind_of?(Hash)
        raise TypeError, "wrong argument type #{limit.class} (expected Hash)"
      end

      if (label_features = @documents.assoc(label))
        label_features[1] = features
      else
        @documents << [label, features]
      end
    end

    def do_clustering(method = Analyzer::REPEATED_BISECTION)
      analyzer = Analyzer.new
      analyzer.set_cluster_size_limit(@cluster_size_limit) if @cluster_size_limit
      analyzer.set_eval_limit(@eval_limit) if @eval_limit
      analyzer.set_output_similairty_point(@output_similairty_point) if @output_similairty_point

      feature_set = []

      @documents.each_with_index do |label_features, i|
        doc = Document.new(i)

        label_features[1].each do |feature, value|
          feature_set << feature unless feature_set.include?(feature)
          doc.add_feature(feature_set.index(feature), value)
        end

        analyzer.add_document(doc)
      end

      analyzer.do_clustering(method)

      result = []

      while (cluster = analyzer.get_next_result)
        if @output_similairty_point
          result << cluster.map {|doc_id, point| [@documents[doc_id][0], point] }
        else
          result << cluster.map {|doc_id| @documents[doc_id][0] }
        end
      end

      return result
    end
  end
end

import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
       
        # Create sets to store the words
        self.positives = set()
        self.negatives = set()
        
        # Open file with positive words, iterate over each line in file, and if the line is not a comment add word to set
        with open(positives) as pos:
            for line in pos:
                if not line.startswith(";") and not line.startswith(" "):
                    self.positives.add(line.strip())
                
        # Same as above but treating the file with negative words
        with open(negatives) as neg:
            for line in neg:
                if not line.startswith(";") and not line.startswith(" "):
                    self.negatives.add(line.strip())

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # Use tokenizer to split text into a list of words
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        # Iterate over list and check if word is either on the negative or positive list, assigning score
        score = 0
        for item in tokens:
            if str.lower(item) in self.positives:
                score += 1
            if str.lower(item) in self.negatives:
                score -= 1

        return score

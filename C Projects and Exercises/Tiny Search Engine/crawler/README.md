# CS50 Lab 4
## CS50 Spring 2023

### crawler

The crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limiting itself to some threshold number of hops from the seed URL, and avoiding visiting any given URL more than once. It saves the pages, and the URL and depth for each, in files. When the crawler process is complete, the indexing of the collected documents can begin.

## Data structures 

We use two data structures: a 'bag' of pages that need to be crawled, and a 'hashtable' of URLs that we have seen during our crawl.
Both start empty.
The size of the hashtable (slots) is impossible to determine in advance, so we use 200.

## Control flow

The Crawler is implemented in one file `crawler.c`, with four functions.

### main

The `main` function simply calls `parseArgs` and `crawl`, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `seedURL`, normalize the URL and validate it is an internal URL
* for `pageDirectory`, call `pagedir_init()`
* for `maxDepth`, ensure it is an integer in specified range
* if any trouble is found, print an error to stderr and exit non-zero.

### crawl

Do the real work of crawling from `seedURL` to `maxDepth` and saving pages in `pageDirectory`.
Pseudocode:

	initialize the hashtable and add the seedURL
	initialize the bag and add a webpage representing the seedURL at depth 0
	while bag is not empty
		pull a webpage from the bag
		fetch the HTML for that webpage
		if fetch was successful,
			save the webpage to pageDirectory
			if the webpage is not at maxDepth,
				pageScan that HTML
		delete that webpage
	delete the hashtable
	delete the bag

### pageScan

This function implements the *pagescanner* mentioned in the design.
Given a `webpage`, scan the given page to extract any links (URLs), ignoring non-internal URLs; for any URL not already seen before (i.e., not in the hashtable), add the URL to both the hashtable `pages_seen` and to the bag `pages_to_crawl`.
Pseudocode:

	while there is another URL in the page
		if that URL is Internal,
			insert the webpage into the hashtable
			if that succeeded,
				create a webpage_t for it
				insert the webpage into the bag
		free the URL
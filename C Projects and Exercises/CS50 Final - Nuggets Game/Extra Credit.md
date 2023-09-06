## Extra Credit 

While the Nuggets project requires a lot of work, there are always some brave(?) teams that want to reach a little further. My first warning is always:

> Be sure you have everything that is required complete before considering more.  Extra credit will not be worth the effort if it only serve to make up for points lost due to a missing requirement or surprising test failure.

That said, we do have opportunities for up to **10 points** extra credit.  Extra credit is always graded strictly, so proceed carefully.

### How to include extra credit functionality 

Any new extra credit functionality should be included in the final `DESIGN.md` and `IMPLEMENTATION.md` documents, `Makefile` (if needed), and included in your testing scripts/code. It should be clearly identified as such in the documentation. For example:\

> **Extra Credit:**
> description of the additional functionality.
> ...
> ---

The `---` produces a horizontal line in Markdown.

If you need to update the `Makefile` or testing scripts/code, clearly identify the updates.

### Options for extra credit

Any of the following enhancements would qualify for extra credit.

1. Provide optional server arguments: 
	* `--gold 500`
	* `--minpiles 15`‌
	* `--maxpiles 40`
	then change optional seed parameter to 
	* `--seed 123`
This requires use of the `getopt( )` function.
2. Set a range limit on vision, e.g., a diameter of five spots.
2. Allow players to steal gold from other players by stepping onto them.  Optionally alert players when they "hear someone coming."
3. Player who quits before the end of the game gives up their gold, leaving a new pile at their last location.
4. Decrease players' *lifetime* with every move, causing them to exit when lifetime reaches zero; sprinkle *food* around the map; moving onto food increases their lifetime.
6. Provide another server argument `--plain` that disables all the extra credit capabilities. This requires use of the `getopt( )` function.
 






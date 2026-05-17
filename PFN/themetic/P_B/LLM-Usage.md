# Generative AI Usage Disclosure

## Service Name
Claude 4.7 Opus

## How it was used
I utilized Generative AI as my primary tool for solving, drafting, and typesetting the responses for both Problem A and Problem B. The AI was used end-to-end to generate the solutions, refine the technical content, and format the final document.
I use AI agents in the following iteration: 
1. Give it the original question description, and let it generate the first answer prototype.
2. Review the current answer, confirming the correctness.
3. If the answer is unsatisfying, give it prompt for adjust the answer in the way I want and back to step 2. Otherwise (the answer looks good to me), stepping to step 4.
4. Stop and submit the answer.

## Prompts Entered

### Problem A Prompts

    "Solve the problems showed in the screenshot."

    "The word is too hard in the report. Use more easier word. And also, many hard concept has not been described or described in detail. After modifcation, shrink the content to fulfill the 2 A4 page constraint."

    "The content is still too much. The table uses much space"

    "Rewrite it in typst (don't delete the old one)"

    "Looks good. Add more spaces between sections, and make the font a little bit larger."

    "Need you to give two modifications of A-2: 1. Write more details about the PageRank (and why it is related to the problem). 2. About the optimizations of SpMV: you should specialize to the PageRank problem, not for generic case. For example, Multi-thread SpMV should not be considered specialization."

    "No, don't modify the pagerank itself. The specialization is, only modify the algorithm of SpMV for optimization. And this modification is for pagerank but not generic."

    "Looks good. Content looks OK. But the sentence of this report looks like an answer to my request. Remember this report is the the report to the 'problems' (in the screenshot before), but not me. Adjust the sentences and formats"

### Problem B Prompts

    "Solve the problem in this description (Please answer the following within two A4 pages total. Use Typst for writing. Discuss the architectural characteristics required for an accelerator designed for LLM inference, as well as the software stack needed to support that architecture. ## Additional Requirements - Please start from easy to hard. From simple concept and then go deeper - You should write in a report format, with clear structure. - Do not use hard words - If some hard concept you have to use, describe them in detail first)"

    "I think you want to include too much information, make too much points. Cut off some points and focus on the rest, give them more details and go deeper on each of them"
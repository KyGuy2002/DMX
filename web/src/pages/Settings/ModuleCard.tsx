import { Button } from "@/components/ui/button"


export default function ModuleCard(props: {name: string}) {

    return (
        <Button variant="outline" className="h-30 flex flex-col">

            <p>{props.name}</p>

        </Button>
    )
}